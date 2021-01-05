#include "dc_motor.h"
#include <cstring>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

DCMotor::DCMotor(int pin, int i2c_addr) : pin_(pin), i2c_addr_(i2c_addr)
{
    // Open an I2C bus.
    fd_ = wiringPiI2CSetup(i2c_addr_);
    // Error handling
    if (fd_ == -1)
    {
        std::ostringstream err_msg;
        err_msg << "wiringPiI2CSetup(" << i2c_addr_ << ") has failed: " << strerror(errno) << "\n";
        throw std::runtime_error(err_msg.str());
    }
}

DCMotor::~DCMotor()
{
    stop(); // Stop the shaft from spining.
    if (fd_ >= 0) close(fd_); // Close the I2C bus
}

void DCMotor::run(int speed)
{
    // Compute the command speed between 0 and 255.
    int cmd_speed(speed < 0 ? -speed : speed);
    if (speed > 255) { cmd_speed = 255; }
    // Build and send the message to the driver.
    int registry(0x02); // Rotate clockwise.
    int message((cmd_speed << 8) | (pin_ & 0xFF));
    if (speed < 0) registry = 0x03; // Rotate counter clockwise.
    wiringPiI2CWriteReg16(fd_, registry, message);
}

void DCMotor::shaftBreak() { wiringPiI2CWriteReg16(fd_, 0x00, pin_); }

void DCMotor::stop() { wiringPiI2CWriteReg16(fd_, 0x01, pin_); }
