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

Lidar::Lidar(std::shared_ptr<DCMotor> pivot,
             std::shared_ptr<ShaftEncoder> encoder,
             std::shared_ptr<IRSensor> sensor,
             float max_angle,
             float min_angle,
             int motor_speed) :
    pivot_(pivot),
    encoder_(encoder),
    sensor_(sensor),
    motor_speed_(100),
    min_angle_(normalize(min_angle)),
    max_angle_(normalize(max_angle)),
    angle_increment_(0.1),
    motor_sleep_(20),
    motor_stuck_(100),
    unstuck_cooldown_(500),
    is_measuring_(false),
    meas_mtx_(),
    meas_thread_(),
    scan_()
{
    assert(pivot != nullptr);
    assert(encoder != nullptr);
    assert(sensor != nullptr);
    assert(0 < motor_speed_);
    // Ensure the min is inferior to the max
    if (max_angle_ < min_angle_)
    {
        float tmp(min_angle_);
        min_angle_ = max_angle_;
        max_angle_ = tmp;
    }
}

Lidar::~Lidar() { stop(); }

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
    if (!is_measuring_.load())
    {
        is_measuring_.store(true);
        auto task = std::bind(&Lidar::measureTask, this);
        meas_thread_ = std::thread(task); // Launch the thread
    }
}

void Lidar::stop()
{
    if (is_measuring_.load())
    {
        is_measuring_.store(false); // End the thread
        meas_thread_.join(); // Wait for the thread to end
    }
}

void Lidar::measureTask()
{
    bool do_mes(false);
    float pos;
    float old_pos(1E10);
    int last_step;
    int speed(motor_speed_);
    std::list<Lidar::Measure> mes_tab;
    std::chrono::system_clock::time_point last_move;
    auto last_change(std::chrono::system_clock::now());

    pivot_->run(speed);
    while (is_measuring_.load())
    {
        pos = encoder_->measurePosition();
        int step(encoder_->measureIncrements());
        auto now(std::chrono::system_clock::now());
        if (step != last_step) // Reset the stuck countdown
        {
            last_move = now;
            last_step = step;
        }
        bool is_stuck(motor_stuck_ < std::chrono::duration_cast<std::chrono::milliseconds>(now - last_move));
        bool has_cooled_down(unstuck_cooldown_ <
                             std::chrono::duration_cast<std::chrono::milliseconds>(now - last_change));
        // Manage the rotation
        if (pos < min_angle_ || (is_stuck && has_cooled_down && speed < 0))
        {
            // Send the motor back and enable measurements
            speed = motor_speed_;
            last_change = std::chrono::system_clock::now();
            pivot_->run(speed);
            do_mes = true;
        }
        else if (pos > max_angle_ || (is_stuck && has_cooled_down && 0 < speed))
        {
            // Send the motor back, store then clear the local measurements and disable measurements
            speed = -motor_speed_;
            pivot_->run(speed);
            if (!mes_tab.empty())
            {
                std::lock_guard<std::mutex> tracker_lock(meas_mtx_);
                scan_ = mes_tab;
            }
            mes_tab.clear();
            do_mes = false;
        }
        // Make measurement
        if (do_mes && angle_increment_ < std::abs(old_pos - pos))
        {
            old_pos = pos;
            Lidar::Measure measure{.orientation = encoder_->measurePosition(), .distance = sensor_->measure()};
            mes_tab.push_back(measure);
        }
        std::this_thread::sleep_for(motor_sleep_);
    }
    pivot_->stop();
}
