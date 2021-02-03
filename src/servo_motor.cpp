#include "servo_motor.h"
#include <cmath>
#include <cstring>
#include <iostream>
#include <softPwm.h>
#include <sstream>
#include <stdexcept>
#include <wiringPi.h>

ServoMotor::ServoMotor(int nb_pin, int min_duty, int max_duty) :
    pin_(nb_pin),
    angle_(NAN),
    min_(min_duty),
    delta_(max_duty - min_duty)
{
    pinMode(nb_pin, OUTPUT);
    digitalWrite(nb_pin, LOW);
    // Error handling
    if (softPwmCreate(nb_pin, 0, 100) != 0)
    {
        std::ostringstream err_msg;
        err_msg << "softPwmCreate has failed: " << strerror(errno) << "\n";
        throw std::runtime_error(err_msg.str());
    }

    softPwmWrite(nb_pin, min_);
}

float ServoMotor::angle() const { return angle_; }

void ServoMotor::rotate(float angle)
{
    int duty_cycle(delta_ * (angle / M_PI + 0.5) + min_);

    // Clip the duty cycle
    if (duty_cycle < min_) { duty_cycle = min_; }
    if (duty_cycle > min_ + delta_) { duty_cycle = min_ + delta_; }

    softPwmWrite(pin_, duty_cycle); // Apply the new duty cycle
    angle_ = ((duty_cycle - min_) / delta_ - 0.5) * M_PI; // Store the actual desired angle
}
