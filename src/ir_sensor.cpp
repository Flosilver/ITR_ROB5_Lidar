#include "ir_sensor.h"

#include <cstring>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <unistd.h>
#include <math.h>

#include <wiringPi.h>
#include <wiringPiI2C.h>

using namespace std;

IRSensor::IRSensor(int addr, int nb_pin) : pin_(nb_pin), i2c_addr_(addr)
{
    // TODO: shit pin gestion
    fd_ = wiringPiI2CSetup(i2c_addr_);
    // Error handling
    if (fd_ == -1)
    {
        ostringstream err_msg;
        err_msg << "wiringPiI2CSetup(" << i2c_addr_ << ") has failed: " << strerror(errno) << "\n";
        throw std::runtime_error(err_msg.str());
    }
}

IRSensor::~IRSensor()
{
    // TODO: liberation de la pin
    if (fd_ >= 0) close(fd_);
    cout << "\tdest IR sensor" << endl;
}

float IRSensor::measure() const
{
    int reg = pin_ + 0x20;

    wiringPiI2CWrite(fd_, reg);
    int res = wiringPiI2CReadReg16(fd_, reg);
    // Error handling
    if (res < 0)
    {
        ostringstream err_msg;
        err_msg << "wiringPiI2CReadReg16(" << fd_ << ", " << reg << ") has failed: " << strerror(errno) << "\n";
        throw runtime_error(err_msg.str());
    }

    return measToDist(((float)res) / 1000.);
}

float IRSensor::measToDist(const float mes) const{
    if (mes < 0.35) return 80.;
    if (mes > 2.288) return 9.;

    return 23.5 * pow(mes,-1.15);
}
