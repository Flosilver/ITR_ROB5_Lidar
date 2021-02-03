#include "proximity_tracker.h"
#include <cassert>
#include <chrono>
#include <functional>
#include <iostream>
#include <list>

ProximityTracker::ProximityTracker(std::shared_ptr<Lidar> lidar,
                                   std::shared_ptr<CameraPanTilt> designator,
                                   int scan_cooldown) :
    is_running_(false),
    tracker_thread_(),
    tracker_mutex_(),
    lidar_(lidar),
    camera_(designator),
    track_sleep_(scan_cooldown)
{
    assert(lidar_ != nullptr);
    assert(camera_ != nullptr);
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
    {
        std::lock_guard<std::mutex> tracker_lock(tracker_mutex_);
        is_running_ = false;
    }
    if (tracker_thread_.joinable()) tracker_thread_.join();
}

void ProximityTracker::track()
{
    std::list<Lidar::Measure> ir_scan;
    Lidar::Measure min_measure;
    bool keep_running(true);

    // Start utility objects
    lidar_->start();
    camera_->display();

    while (keep_running)
    {
        // Search the nearest obstacle
        min_measure.orientation = NAN;
        min_measure.distance = 1E10F;
        ir_scan = lidar_->scan();
        for (Lidar::Measure& measure : ir_scan)
        {
            if (measure.distance < min_measure.distance) min_measure = measure;
        }

        // Rotate to the nearest obstacle
        if (!std::isnan(min_measure.distance) && min_measure.distance < 0.75F) // Ensure an object has been detected
        {
            std::cout << "Object detected at " << min_measure.orientation / M_PI * 180.0 << "° and "
                      << min_measure.distance << " m\n";
            camera_->rotatePan(min_measure.orientation);
        }

        // Update the loop flag
        {
            std::lock_guard<std::mutex> tracker_lock(tracker_mutex_);
            keep_running = is_running_;
        }

        std::this_thread::sleep_for(track_sleep_);
    }

    // Stop utility objects
    camera_->stopDisplay();
    lidar_->stop();
}
