#include "lidar.h"
#include <iostream>
#include <chrono>
#include <cassert>

using namespace std;

Lidar::Lidar(std::shared_ptr<MotorCtrl> pivot, std::shared_ptr<IRSensor> sensor, float l_edge, float r_edge):
    pivot_(pivot),
    sensor_(sensor),
    left_edge_(l_edge),
    right_edge_(r_edge)
{
    assert(pivot != nullptr);
    assert(sensor != nullptr);
}

Lidar::~Lidar() {
    // Stop the motor
    pivot_->motor_stop();
}

std::vector<Lidar::Measure> Lidar::update(float increment, int sleep)
{
    std::vector<Lidar::Measure> list;
    auto period_duration = std::chrono::milliseconds(sleep);

    std::shared_ptr<ShaftEncoder> encoder = pivot_->encoder();

    std::cout << "brones: " << right_edge_ << ", " << left_edge_ << "\n";

    for (float i=right_edge_ ; i<=left_edge_ ; i+=increment)
    {
        pivot_->rotateAsync(i);
        std::this_thread::sleep_for(period_duration);
        std::cout <<"sleep " << i << "\n";
        Lidar::Measure mes;
        mes.pos = encoder->measurePosition();
        mes.meas = sensor_->measure();

        list.push_back(mes);
    }
    pivot_->rotateAsync(right_edge_);
    std::this_thread::sleep_for(period_duration);
    return list;
}