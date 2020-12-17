#include "lidar.h"
#include <cassert>
#include <chrono>
#include <cmath>
#include <iostream>
#include <functional>

using namespace std;

Lidar::Lidar(std::shared_ptr<DCMotor> pivot, std::shared_ptr<ShaftEncoder> encoder, std::shared_ptr<IRSensor> sensor, float max_angle, float min_angle) :
    pivot_(pivot),
    encoder_(encoder),
    sensor_(sensor),
    min_angle_(normalize(min_angle)),
    max_angle_(normalize(max_angle)),
    angle_increment_(0.1),
    motor_sleep_(20)
{
    assert(pivot != nullptr);
    assert(encoder != nullptr);
    assert(sensor != nullptr);
    // Ensure the min is inferior to the max
    if (max_angle_ < min_angle_)
    {
        float tmp(min_angle_);
        min_angle_ = max_angle_;
        max_angle_ = tmp;
    }
}

Lidar::~Lidar(){ stop(); }

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

std::list<Lidar::Measure> Lidar::scan()
{
    std::lock_guard<std::mutex> tracker_lock(meas_mtx_);
    return scan_;
}

void Lidar::start()
{
    std::lock_guard<std::mutex> tracker_lock(meas_mtx_);
    if (!is_meas_)
    {
        is_meas_ = true;
        auto task = std::bind(&Lidar::measureTask, this);
        meas_thread_ = std::thread(task);
    }
}

void Lidar::stop()
{
    if (is_meas_)
    {
        {
            std::lock_guard<std::mutex> tracker_lock(meas_mtx_);
            is_meas_ = false;
        }
        meas_thread_.join();
    }
}

void Lidar::measureTask()
{
    bool keep_meas(true);
    bool do_mes(false);
    float pos;
    float old_pos = 1e10;
    int speed = 80;
    int max_speed = speed;
    std::list<Lidar::Measure> mes_tab;

    while(keep_meas){
        pivot_->run(speed);
        pos = encoder_->measurePosition();
        std::cout << pos << "\n";

        if (pos < min_angle_){
            speed = max_speed;
            do_mes = true;
        }
        if (pos > max_angle_){
            speed = -max_speed;
            do_mes = false;
            if (!mes_tab.empty())
            {
                std::lock_guard<std::mutex> tracker_lock(meas_mtx_);
                scan_ = mes_tab;
            }
            mes_tab.clear();
        }
        if (do_mes && std::abs(old_pos - pos) > angle_increment_)
        {
            std::cout << "dedans :X\n";
            old_pos = pos;
            Lidar::Measure measure{.orientation = encoder_->measurePosition(), .distance = sensor_->measure()};
            mes_tab.push_back(measure);
        }
        {
            std::lock_guard<std::mutex> tracker_lock(meas_mtx_);
            keep_meas = is_meas_;
        }
        std::this_thread::sleep_for(motor_sleep_);
    }
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
