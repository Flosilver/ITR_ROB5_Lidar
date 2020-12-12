#ifndef __PROXIMITY_TRACKER_H__
#define __PROXIMITY_TRACKER_H__

#include <memory>
#include <mutex>
#include <thread>
#include "lidar.h"
#include "servo_motor.h"

class ProximityTracker
{
    bool is_running_;
    std::thread tracker_thread_;
    std::mutex tracker_mutex_;
    std::shared_ptr<Lidar> lidar_;
    std::shared_ptr<ServoMotor> designator_;

public:
    ProximityTracker() = default;
    ProximityTracker(std::shared_ptr<Lidar> lidar, std::shared_ptr<ServoMotor> designator);
    ~ProximityTracker();

    std::shared_ptr<const Lidar> lidar() const { return lidar_; }
    std::shared_ptr<Lidar> lidar() { return lidar_; }

    std::shared_ptr<const ServoMotor> designator() const { return designator_; }
    std::shared_ptr<ServoMotor> designator() { return designator_; }

    bool isRunning() const { return is_running_; }

    void start();
    void stop();
private:
    void track();
};
#endif // __PROXIMITY_TRACKER_H__
