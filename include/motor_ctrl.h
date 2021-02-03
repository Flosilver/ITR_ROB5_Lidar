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
    float desired_orientation_; ///< The desired shaft orientation (in radians).
    const unsigned int sp_; ///< The sampling period (in microseconds).
    const float kp_; ///< The proportional gain of the PI corrector.
    const float ki_; ///< The integral gain of the PI corrector.
    const float anti_windup_; ///< The threshold at which to freeze the integral.
    const int max_speed_;

public:
    /**
     * Construct a new motor controller.
     *
     * @param pin The pin wired to the DC motor.
     * @param i2c_addr The I2C address of the driver.
     * @param encoder The encoder attached on the shaft.
     * @param kp The proportional gain of the PI corrector.
     * @param ki The integral gain of the PI corrector.
     * @param sampling_period The sampling period (in micro seconds).
     */
    MotorCtrl(int pin,
              int i2c_addr,
              std::shared_ptr<ShaftEncoder> encoder,
              float kp = 500.0,
              float ki = 2,
              float anti_windup = 50.0,
              int max_speed = 100,
              unsigned int sampling_period = 1000);

    MotorCtrl(const MotorCtrl& other) = delete;

    /**
     * Destroy the motor controller.
     *
     * Stop the control loop and join the control thread. The motor is also
     * stopped.
     */
    virtual ~MotorCtrl();

    MotorCtrl& operator=(const MotorCtrl& other) = delete;

    /**
     * Rotate the shaft to the desired orientation.
     *
     * The rotation is performed asynchronously, thus the method returns
     * instantly. There is no garanty of the position of the shaft when the
     * method returns.
     *
     * NOTE: The orientation is relative to the encoder's home.
     *
     * @param orientation The desired orientation of the shaft (in radians).
     */
    void rotateAsync(float orientation);

    /**
     * Get the encoder shaft.
     *
     * @return The encoder shaft.
     */
    std::shared_ptr<const ShaftEncoder> encoder() const { return encoder_; }
    /**
     * @overload encoder()
     */
    std::shared_ptr<ShaftEncoder> encoder() { return encoder_; }

    /**
     * The sampling period (in microseconds).
     *
     * @return The sampling period (in microseconds).
     */
    int samplingPeriode() const { return sp_; }

    /**
     * The desired orientation of the shaft.
     *
     * @return The desired orientation of the shaft (in radians).
     */
    float desiredOrientation()
    {
        std::lock_guard<std::mutex> ctrl_lock(ctrl_mtx_);
        return desired_orientation_;
    }

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
