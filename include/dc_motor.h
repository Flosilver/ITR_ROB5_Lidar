#ifndef __DCMOTOR_H__
#define __DCMOTOR_H__

class DCMotor
{
    int pin_; ///< The physical pin on the driver.
    int i2c_addr_; ///< The I2C address of the driver.
    int fd_; ///< The file descriptor of the I2C bus.

public:
    /**
     * Construct a dummy DCMotor
     */
    DCMotor() = default;

    /**
     * Construct a new DCMotor.
     *
     * Open an I2C bus to the driver.
     *
     * @param pin The physical pin on the driver.
     * @param i2c_addr The I2C address of the driver.
     */
    DCMotor(int pin, int i2c_addr);

    /**
     * Destroy the DCMotor.
     *
     * Close the I2C bus.
     */
    virtual ~DCMotor();

    /**
     * Run the motor at the given speed.
     *
     * @param speed The shaft speed in [-255, 255].
     */
    void run(int speed);

    /**
     * Make the motor break.
     */
    void shaftBreak();

    /**
     * Stop the motor.
     */
    void stop();
};

#endif
