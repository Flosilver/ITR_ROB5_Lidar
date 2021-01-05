#include "lidar.h"
#include <cassert>
#include <chrono>
#include <cmath>
#include <functional>
#include <iostream>

float Lidar::normalize(float angle)
{
    float normalized_angle(angle);
    while (normalized_angle < -M_PI)
        normalized_angle += 2 * M_PI;
    while (normalized_angle >= M_PI)
        normalized_angle -= 2 * M_PI;
    return normalized_angle;
}

Lidar::Lidar(std::shared_ptr<IRSensor> sensor, float max_angle, float min_angle) :
    sensor_(sensor),
    min_angle_(normalize(min_angle)),
    max_angle_(normalize(max_angle)),
    angle_increment_(0.1),
    scan_()
{
    assert(sensor != nullptr);
    // Ensure the min is inferior to the max
    if (max_angle_ < min_angle_)
    {
        float tmp(min_angle_);
        min_angle_ = max_angle_;
        max_angle_ = tmp;
    }
}

Lidar::~Lidar() {}

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
