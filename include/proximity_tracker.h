#ifndef __PROXIMITY_TRACKER_H__
#define __PROXIMITY_TRACKER_H__

#include <memory>
#include <mutex>
#include <thread>
#include "lidar.h"
#include "camera_pan_tilt.h"

class ProximityTracker
{
    bool is_running_;
    std::thread tracker_thread_;
    std::mutex tracker_mutex_;
    std::shared_ptr<Lidar> lidar_;
    std::shared_ptr<CameraPanTilt> camera_;

public:
    ProximityTracker() = default;
    ProximityTracker(std::shared_ptr<Lidar> lidar, std::shared_ptr<CameraPanTilt> designator);
    ~ProximityTracker();

    std::shared_ptr<const Lidar> lidar() const { return lidar_; }
    std::shared_ptr<Lidar> lidar() { return lidar_; }

    std::shared_ptr<const CameraPanTilt> designator() const { return camera_; }
    std::shared_ptr<CameraPanTilt> designator() { return camera_; }

    bool isRunning() const { return is_running_; }

    void start();
    void stop();
private:
    void track();
};
#endif // __PROXIMITY_TRACKER_H__
