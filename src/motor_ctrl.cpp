#include "motor_ctrl.h"
#include <cassert>
#include <chrono>
#include <cmath>
#include <functional>

MotorCtrl::MotorCtrl(int pin, int i2c_addr, std::shared_ptr<ShaftEncoder> encoder, float kp = 1, float ki = 1) :
    DCMotor(pin, i2c_addr),
    encoder_(encoder),
    kp_(kp),
    ki_(ki),
    desired_orientation_(0.0),
    err_int_(0.0)
{
    assert(pin >= 0);
    assert(i2c_addr >= 0);
    assert(encoder != nullptr);
    assert(kp >= 0.0);
    assert(ki >= 0.0);
    // Start the control thread
    auto task = std::bind(MotorCtrl::control, this);
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
    motor_stop();
}

void MotorCtrl::rotateAsync(float orientation)
{
    std::lock_guard<std::mutex> ctrl_lock(ctrl_mtx_);
    desired_orientation_ = orientation;
}

void MotorCtrl::control()
{
    float dt = sp_ * 1e-6;
    while (is_controlled_)
    {
        // Copy the disered orientation
        float des;
        {
            std::lock_guard<std::mutex> ctrl_lock(ctrl_mtx_);
            des = desired_orientation_;
        }
        // Compute the PI corrector
        float mes = encoder_->measurePosition();
        float error = des - mes;
        float float_command = kp_ * (error + ki_ * err_int_);
        if (-anti_windup_ < float_command && float_command < anti_windup_)
        {
            err_int_ += error * dt;
            float_command = kp_ * (error + ki_ * err_int_);
        }
        // Send the command to the motor
        int int_command = (int)std::round(float_command);
        run(int_command);

        std::this_thread::sleep_for(std::chrono::microseconds(sp_));
    }
}
