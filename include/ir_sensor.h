#ifndef IRSENSOR_H
#define IRSENSOR_H

class IRSensor
{
public:
    IRSensor(int addr, int nb_pin);
    ~IRSensor();

    double measure() const;

private:
    int pin_; ///< Device's pin
    int address_; ///< Device's I2C address
    int fd_; ///< File descriptor of the I2C communication bus
};

#endif
