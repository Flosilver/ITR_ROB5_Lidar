#include "servo_motor.h"
#include <cmath>
#include <cstring>
#include <iostream>
#include <softPwm.h>
#include <sstream>
#include <stdexcept>
#include <wiringPi.h>

using namespace std;

ServoMotor::ServoMotor(int nb_pin, int min_duty, int max_duty) :
    pin_(nb_pin),
    angle_(0.0F),
    min_(min_duty),
    delta_(max_duty - min_duty)
{
    // shit pin gestion: TODO
    pinMode(nb_pin, OUTPUT);
    digitalWrite(nb_pin, LOW);
    cout << "pin mode seted up to OUTPUT with LOW value\n";
    // Error handling
    if (softPwmCreate(nb_pin, 0, 100) != 0)
    {
        ostringstream err_msg;
        err_msg << "softPwmCreate has failed: " << strerror(errno) << "\n";
        throw runtime_error(err_msg.str());
    }

    softPwmWrite(nb_pin, min_);
}

ServoMotor::~ServoMotor()
{
    cout << "\tdest servo moteur" << endl;
    // liberation de la pin: TODO
}

float ServoMotor::angle() const { return angle_; }

void ServoMotor::rotate(float angle)
{
    int duty_cycle = (int)delta_ * angle / 180.0 + min_;
    if (duty_cycle < min_) { duty_cycle = min_; }
    if (duty_cycle > min_ + delta_) { duty_cycle = min_ + delta_; }
    softPwmWrite(pin_, duty_cycle);

    angle_ = (duty_cycle - min_) * 180.0 / delta_;
}
