#include "dc_motor.h"

#include <cstring>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <unistd.h>

#include <wiringPi.h>
#include <wiringPiI2C.h>

using namespace std;

DCMotor::DCMotor(int pin, int i2c_addr): pin_(pin), i2c_addr_(i2c_addr){
    // récupération du file descriptor
    fd_ = wiringPiI2CSetup(i2c_addr_);

    // Error handling
    if (fd_ == -1)
    {
        ostringstream err_msg;
        err_msg << "wiringPiI2CSetup(" << i2c_addr_ << ") has failed: " << strerror(errno) << "\n";
        throw std::runtime_error(err_msg.str());
    }
}

DCMotor::~DCMotor(){
    // fermeture du file descriptor
    if (fd_ >= 0) close(fd_);
    cout << "\tdest IR sensor" << endl;
}

int DCMotor::get_motor_cmd(int speed) const{
    return (speed << 16) | (pin_ & 0xFF);
}

/**
* Run the motor clockwise.
**/
void DCMotor::run_cw(unsigned int speed){
    if (speed > 255) speed = 255;
    int cmd = get_motor_cmd(speed);
    wiringPiI2CWriteReg16(fd_, CMD_CW, cmd);
}

/**
* Run the motor counter clockwise
**/
void DCMotor::run_ccw(unsigned int speed){
    if (speed > 255) speed = 255;
    int cmd = get_motor_cmd(speed);
    wiringPiI2CWriteReg16(fd_, CMD_CCW, cmd);
}

void DCMotor::run(int speed){
    if (speed > 255) speed = 255;
    if (speed < -255) speed = -255;

    if (speed >= 0){
        run_cw(speed);
    }
    else{
        run_ccw(-speed);
    }
}

void DCMotor::motor_break(){
    int cmd = get_motor_cmd(0);
    wiringPiI2CWriteReg16(fd_, CMD_BREAK, cmd);
}

void DCMotor::motor_stop(){
    int cmd = get_motor_cmd(0);
    wiringPiI2CWriteReg16(fd_, CMD_STOP, cmd);
}