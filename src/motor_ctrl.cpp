#include "motor_ctrl.h"
#include <cassert>
#include <chrono>
#include <cmath>
#include <functional>
#include <iostream>

MotorCtrl::MotorCtrl(int pin,
                     int i2c_addr,
                     std::shared_ptr<ShaftEncoder> encoder,
                     float kp,
                     float ki,
                     float anti_windup) :
    DCMotor(pin, i2c_addr),
    encoder_(encoder),
    ctrl_thread_(),
    ctrl_mtx_(),
    is_controlled_(true),
    desired_orientation_(0.0F),
    sp_(1E3),
    kp_(kp),
    ki_(ki),
    anti_windup_(anti_windup)
{
    assert(pin >= 0);
    assert(i2c_addr >= 0);
    assert(encoder != nullptr);
    assert(kp >= 0.0);
    assert(ki >= 0.0);
    assert(anti_windup >= 0.0);
    // Start the control thread
    auto task = std::bind(&MotorCtrl::control, this);
    ctrl_thread_ = std::thread(task);
}

MotorCtrl::~MotorCtrl()
{
    // Stop the thread
    {
        std::lock_guard<std::mutex> ctrl_lock(ctrl_mtx_);
        is_controlled_ = false;
    }
    ctrl_thread_.join();
    // Stop the motor
    stop();
}

void MotorCtrl::rotateAsync(float orientation)
{
    std::lock_guard<std::mutex> ctrl_lock(ctrl_mtx_);
    desired_orientation_ = orientation;
}

void MotorCtrl::control()
{
    float dt = sp_ * 1e-6;
    auto period_duration = std::chrono::microseconds(sp_);
    float des(0.0F); // The desired shaft orientation (in radians).
    float error_int(0.0F); // The integrated error (in radians.seconds).

    while (is_controlled_)
    {
        // Copy the disered orientation
        {
            std::lock_guard<std::mutex> ctrl_lock(ctrl_mtx_);
            if (des != desired_orientation_) error_int = 0;
            des = desired_orientation_;
        }
        // Compute the PI corrector
        float mes(encoder_->measurePosition());
        float error(des - mes);
        float float_command(kp_ * (error + ki_ * error_int));
        if (-anti_windup_ < float_command && float_command < anti_windup_)
        {
            error_int += error * dt;
            float_command = kp_ * (error + ki_ * error_int);
        }
        // Send the command to the motor
        int int_command = (int)std::round(float_command);
        run(int_command);

        std::this_thread::sleep_for(period_duration);
    }
}
