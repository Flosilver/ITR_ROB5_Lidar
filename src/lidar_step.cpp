#include "lidar_step.h"
#include <cassert>
#include <chrono>
#include <cmath>

LidarStep::LidarStep(std::shared_ptr<MotorCtrl> pivot,
                     std::shared_ptr<IRSensor> sensor,
                     float max_angle,
                     float min_angle) :
    Lidar(sensor, min_angle, max_angle),
    pivot_(pivot),
    motor_sleep_(300)
{
    assert(pivot != nullptr);
}

std::list<LidarStep::Measure> LidarStep::scan()
{
    std::list<LidarStep::Measure> list;
    std::shared_ptr<ShaftEncoder> encoder = pivot_->encoder();

    for (float angle = min_angle_; angle <= max_angle_; angle += angle_increment_)
    {
        // Rotate the sensor
        pivot_->rotateAsync(angle);
        std::this_thread::sleep_for(motor_sleep_);
        // Measure the distance to an obstacle and store the measurement
        LidarStep::Measure measure{.orientation = encoder->measurePosition(), .distance = sensor_->measure()};
        list.push_back(measure);
    }
    // Rotate back to the starting angle
    pivot_->rotateAsync(min_angle_);
    std::this_thread::sleep_for(motor_sleep_);

    return list;
}
