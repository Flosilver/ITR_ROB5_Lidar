#include "ir_sensor.h"
#include <cstring>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <unistd.h>
#include <wiringPi.h>

using namespace std;

IRSensor::IRSensor(int addr, int nb_pin) : address_(addr), pin_(nb_pin)
{
    // TODO: shit pin gestion
    fd_ = wiringPiI2CSetup(address_);
    if (fd_ == -1)
    {
        ostringstream err_msg;
        err_msg << "wiringPiI2CSetup(" << address_ << ") has failed: " << strerror(errno) << "\n";
        throw std::runtime_error(err_msg.str());
    }
}

IRSensor::~IRSensor()
{
    // TODO: liberation de la pin
    if (fd_ >= 0) close(fd_);
    cout << "\tdest IR sensor" << endl;
}

double IRSensor::measure() const
{
    int reg = address_ + 0x30;

    wiringPiI2CWrite(fd_, reg);
    int res = wiringPiI2CReadReg16(fd_, reg);
    // Error handling
    if (res < 0)
    {
        ostringstream err_msg;
        err_msg << "wiringPiI2CReadReg16(" << fd_ << ", " << reg << ") has failed: " << strerror(errno) << "\n";
        throw runtime_error(err_msg.str());
    }

    return ((double)res) / 10.;
}
