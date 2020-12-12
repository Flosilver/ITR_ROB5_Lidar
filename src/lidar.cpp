#include "lidar.h"
#include <cassert>
#include <chrono>
#include <cmath>

using namespace std;

Lidar::Lidar(std::shared_ptr<MotorCtrl> pivot, std::shared_ptr<IRSensor> sensor, float max_angle, float min_angle) :
    pivot_(pivot),
    sensor_(sensor),
    min_angle_(normalize(min_angle)),
    max_angle_(normalize(max_angle)),
    angle_increment_(0.1),
    motor_sleep_(200)
{
    assert(pivot != nullptr);
    assert(sensor != nullptr);
    // Ensure the min is inferior to the max
    if (max_angle_ < min_angle_)
    {
        float tmp(min_angle_);
        min_angle_ = max_angle_;
        max_angle_ = tmp;
    }
}

void Lidar::minAngle(float angle)
{
    min_angle_ = normalize(angle);
    assert(min_angle_ < max_angle_);
}

void Lidar::maxAngle(float angle)
{
    max_angle_ = normalize(angle);
    assert(min_angle_ < max_angle_);
}

std::vector<Lidar::Measure> Lidar::scan()
{
    std::vector<Lidar::Measure> list;
    std::shared_ptr<ShaftEncoder> encoder = pivot_->encoder();

    for (float angle = min_angle_; angle <= max_angle_; angle += angle_increment_)
    {
        // Rotate the sensor
        pivot_->rotateAsync(angle);
        std::this_thread::sleep_for(motor_sleep_);
        // Measure the distance to an obstacle and store the measurement
        Lidar::Measure measure{.orientation = encoder->measurePosition(), .distance = sensor_->measure()};
        list.push_back(measure);
    }
    // Rotate back to the starting angle
    pivot_->rotateAsync(min_angle_);
    std::this_thread::sleep_for(motor_sleep_);

    return list;
}

float Lidar::normalize(float angle)
{
    float normalized_angle(angle);
    while (normalized_angle < -M_PI)
        normalized_angle += 2 * M_PI;
    while (normalized_angle >= M_PI)
        normalized_angle -= 2 * M_PI;
    return normalized_angle;
}
