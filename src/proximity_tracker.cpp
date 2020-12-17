#include "proximity_tracker.h"
#include <cassert>
#include <functional>

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
    std::vector<Lidar::Measure> ir_scan;
    Lidar::Measure min_measure;
    bool keep_running(true);

    while (keep_running)
    {
        min_measure.distance = 1e10;
        ir_scan = lidar_->scan();
        for (Lidar::Measure& measure : ir_scan)
            if (measure.distance < min_measure.distance) min_measure = measure;
        if (min_measure.distance < 0.6) designator_->rotate(min_measure.orientation);
        
        {
            std::lock_guard<std::mutex> tracker_lock(tracker_mutex_);
            keep_running = is_running_;
        }
    }
}
