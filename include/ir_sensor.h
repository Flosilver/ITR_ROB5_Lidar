#ifndef IRSENSOR_H
#define IRSENSOR_H

class IRSensor
{
public:
    IRSensor(int addr, int nb_pin);
    ~IRSensor();

    float measure() const;

private:
    int pin_; ///< Device's pin
    int i2c_addr_; ///< Device's I2C address
    int fd_; ///< File descriptor of the I2C communication bus

    float meas_to_dist(const float mes) const;
};

#endif
