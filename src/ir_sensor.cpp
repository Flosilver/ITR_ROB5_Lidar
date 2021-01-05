#include "ir_sensor.h"
#include <cstring>
#include <iostream>
#include <math.h>
#include <sstream>
#include <stdexcept>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

IRSensor::IRSensor(int addr, int nb_pin) : pin_(nb_pin), i2c_addr_(addr)
{
    fd_ = wiringPiI2CSetup(i2c_addr_);
    // Error handling
    if (fd_ == -1)
    {
        std::ostringstream err_msg;
        err_msg << "wiringPiI2CSetup(" << i2c_addr_ << ") has failed: " << strerror(errno) << "\n";
        throw std::runtime_error(err_msg.str());
    }
}

IRSensor::IRSensor(IRSensor&& rval) : pin_(rval.pin_), i2c_addr_(rval.i2c_addr_), fd_(rval.fd_)
{
    // Release the resources from the rvalue.
    rval.fd_ = -1;
}

IRSensor::~IRSensor()
{
    // Release the resource.
    if (fd_ >= 0) close(fd_);
}

IRSensor& IRSensor::operator=(IRSensor&& rval)
{
    // Free existing resources.
    if (fd_ >= 0) close(fd_);

    // Copy the data.
    pin_ = rval.pin_;
    i2c_addr_ = rval.i2c_addr_;
    fd_ = rval.fd_;

    // Release the resources from the rvalue.
    rval.fd_ = -1;
    return *this;
}

float IRSensor::measure() const
{
    int registry(pin_ + 0x20);
    wiringPiI2CWrite(fd_, registry);

    int res(wiringPiI2CReadReg16(fd_, registry));
    // Error handling
    if (res < 0)
    {
        std::ostringstream err_msg;
        err_msg << "wiringPiI2CReadReg16(" << fd_ << ", " << registry << ") has failed: " << strerror(errno) << "\n";
        throw std::runtime_error(err_msg.str());
    }

    return measToDist(res / 1000.0F);
}

float IRSensor::measToDist(float mes) const
{
    if (mes < 0.35) return 0.80;
    if (mes > 2.288) return 0.09;

    return 0.235 * std::pow(mes, -1.15);
}
