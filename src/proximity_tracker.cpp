#include "proximity_tracker.h"
#include <cassert>
#include <functional>
#include <list>
#include <iostream>
#include <chrono>

ProximityTracker::ProximityTracker(std::shared_ptr<Lidar> lidar, std::shared_ptr<ServoMotor> designator) :
    is_running_(false),
    tracker_thread_(),
    tracker_mutex_(),
    lidar_(lidar),
    designator_(designator)
{
    assert(lidar_ != nullptr);
    assert(designator_ != nullptr);
}

ProximityTracker::~ProximityTracker() { stop(); }

void ProximityTracker::start()
{
    std::lock_guard<std::mutex> tracker_lock(tracker_mutex_);
    if (!is_running_)
    {
        is_running_ = true;
        auto task = std::bind(&ProximityTracker::track, this);
        tracker_thread_ = std::thread(task);
    }
}

void ProximityTracker::stop()
{
    if (is_running_)
    {
        {
            std::lock_guard<std::mutex> tracker_lock(tracker_mutex_);
            is_running_ = false;
        }
        tracker_thread_.join();
    }
}

void ProximityTracker::track()
{
    std::list<Lidar::Measure> ir_scan;
    Lidar::Measure min_measure;
    bool keep_running(true);
    std::chrono::milliseconds track_sleep(1000);

    lidar_->start();

    while (keep_running)
    {
        min_measure.distance = 1e10;
        ir_scan = lidar_->scan();
        std::cout << "--------------\n";
        for (Lidar::Measure& measure : ir_scan)
        {
            //std::cout << "[" << measure.orientation << ": " << measure.distance << "] ";
            if (measure.distance < min_measure.distance) min_measure = measure;
        }
        std::cout << "[" << min_measure.orientation << ": " << min_measure.distance << "]\n";
        if (min_measure.distance < 0.6) designator_->rotate(min_measure.orientation);
        {
            std::lock_guard<std::mutex> tracker_lock(tracker_mutex_);
            keep_running = is_running_;
        }
        std::this_thread::sleep_for(track_sleep);
    }
}
