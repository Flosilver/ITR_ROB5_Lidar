#ifndef __MOTOR_CTRL_H__
#define __MOTOR_CTRL_H__

#include "dc_motor.h"
#include "shaft_encoder.h"

#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

class MotorCtrl : public DCMotor
{
    std::shared_ptr<ShaftEncoder> encoder_; ///< The shaft orientation sensor.
    std::thread ctrl_thread_; ///< The thread with the control loop.
    std::mutex ctrl_mtx_; ///< The mutex used by the control thread.
    bool is_controlled_; ///< Whether the control loop should keep running or not.
    const long sp_; ///< The sampling period (in microseconds).
    float desired_orientation_; ///< The desired shat orientation (in radians).
    const float kp_; ///< The proportional gain of the PI corrector.
    const float ki_; ///< The integral gain of the PI corrector.
    float error_int_; ///< The integrated error (in radians.seconds).
    float anti_windup_; ///< The threshold at which to freeze the integral.

public:
    /**
     * Construct a new motor controller.
     *
     * @param pin The pin wired to the DC motor.
     * @param i2c_addr The I2C address of the driver.
     * @param encoder The encoder attached on the shaft.
     * @param kp The proportional gain of the PI corrector.
     * @param ki The integral gain of the PI corrector.
     */
    MotorCtrl(int pin,
              int i2c_addr,
              std::shared_ptr<ShaftEncoder> encoder,
              float kp = 200.0,
              float ki = 2,
              float anti_windup = 50.0);

    /**
     * Destroy the motor controller.
     *
     * Stop the control loop and join the control thread. The motor is also
     * stopped.
     */
    virtual ~MotorCtrl();

    /**
     * Rotate the shaft to the desired orientation.
     *
     * The rotation is performed asynchronously, thus the method returns
     * instantly. There is no garanty of the position of the shaft when the
     * method returns.
     *
     * NOTE: The orientation is relative to the encoder's home.
     *
     * @param orientation The disered orientation of the shaft (in radians).
     */
    void rotateAsync(float orientation);

    std::shared_ptr<ShaftEncoder> encoder(){return encoder_;}

private:
    /**
     * Compute and send a velocity command to the motor.
     *
     * Takes the lock only to copy the disered orientation. Then computes a PI
     * corrector and send the resulting command to the motor.
     *
     */
    void control();
};

#endif
