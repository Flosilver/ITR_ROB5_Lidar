#include "motor_ctrl.h"
#include <functional>

MotorCtrl::MotorCtrl(int pin, int i2c_addr, std::string encoder_path, long encoder_cpr, float kp, float ki): DCMotor(pin,i2c_addr), encoder_(encoder_path,encoder_cpr), kp_(kp), ki_(ki), input_(), err_int_()
{
    ctrl_thread_ = std:thread(std::bind(MotorCtrl::control,this)); // oskour
}