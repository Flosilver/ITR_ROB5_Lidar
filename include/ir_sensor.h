#ifndef __IR_SENSOR_H__
#define __IR_SENSOR_H__

class IRSensor
{
    int pin_; ///< The sensor analog pin.
    int i2c_addr_; ///< The DAC I2C address.
    int fd_; ///< File descriptor of the I2C communication bus.
public:
    /**
     * Construct a dummy IRSensor.
     */
    IRSensor() = default;
    IRSensor(const IRSensor& lval) = delete;
    IRSensor(IRSensor&& rval);
    /**
     * Construct a new IRSensor.
     *
     * Open an I2C bus to the DAC.
     *
     * @param addr The sensor analog pin.
     * @param nb_pin The DAC I2C address.
     */
    IRSensor(int addr, int nb_pin);
    /**
     * Destroy the IRSensor.
     *
     * Close the I2C bus to the DAC.
     */
    ~IRSensor();

    IRSensor& operator=(const IRSensor& other) = delete;
    IRSensor& operator=(IRSensor&& rval);

    /**
     * Measure the distance to an obstacle.
     *
     * @return The distance to an obstacle (in meters).
     */
    float measure() const;

private:
    /**
     * Convert the raw measurement to a distance between 0.09 m and 0.8 m.
     *
     * @param mes The raw measurement (in volts).
     * @return The distance (in meters).
     */
    float measToDist(float mes) const;
};

#endif
