#ifndef __STOPED_LIDAR_H__
#define __STOPED_LIDAR_H__

#include <atomic>
#include <chrono>
#include <list>
#include <memory>
#include <mutex>
#include <thread>
#include <cmath>
#include "lidar.h"
#include "dc_motor.h"
#include "ir_sensor.h"
#include "shaft_encoder.h"

class StopedLidar: public Lidar
{
private:
    const std::shared_ptr<DCMotor> pivot_; ///< The pivot that the IR sensor is attached to.
    const std::shared_ptr<ShaftEncoder> encoder_; ///< The encoder
    const int motor_speed_; ///< The motor rotating speed.
    std::chrono::milliseconds motor_sleep_; ///< The time to sleep after a desired angle is given to the motor.
    std::chrono::milliseconds motor_stuck_; ///< The duration threshold to consider the motor to be stuck.
    std::chrono::milliseconds unstuck_cooldown_; ///< The cool down before trying to unstuck the motor.
    std::atomic<bool> is_measuring_; ///< The flag controlling the infinite loop in the thread.
    std::mutex meas_mtx_; ///< The mutex used to synchronize operations around the measurements.
    std::thread meas_thread_; ///< The thread that performs the measurements.
    float stop_angle_; ///< The angular position of the "butée"

public:
    /**
     * Construct a new dummy StopedLidar.
     */
    StopedLidar() = default;
    /**
     * Construct a new StopedLidar.
     *
     * @param pivot The pivot that the IR sensor is attached to.
     * @param sensor The IR sensor measuring the distance.
     */
    StopedLidar(std::shared_ptr<DCMotor> pivot,
          std::shared_ptr<ShaftEncoder> encoder,
          std::shared_ptr<IRSensor> sensor,
          int motor_speed = 50,
          float stop_angle = -M_PI / 4);

    StopedLidar(const StopedLidar& other) = delete;

    /**
     * Destroy the StopedLidar.
     *
     * Stop the measurement.
     */
    ~StopedLidar();

    StopedLidar& operator=(const StopedLidar& other) = delete;

    /**
     * Get the pivot that the IR sensor is attached to.
     *
     * @return the pivot that the IR sensor is attached to.
     */
    std::shared_ptr<const DCMotor> pivot() const { return pivot_; }
    /**
     * @overload pivot()
     */
    std::shared_ptr<DCMotor> pivot() { return pivot_; }

    /**
     * Get the encoder of the motor the IR sensor is attached to.
     *
     * @return the encoder of the motor the IR sensor is attached to.
     */
    std::shared_ptr<const ShaftEncoder> encoder() const { return encoder_; }
    /**
     * @overload encoder()
     */
    std::shared_ptr<ShaftEncoder> encoder() { return encoder_; }

    /**
     * Get the IR sensor measuring the distance.
     *
     * @return the IR sensor measuring the distance.
     */
    std::shared_ptr<const IRSensor> sensor() const { return sensor_; }
    /**
     * @overload sensor()
     */
    std::shared_ptr<IRSensor> sensor() { return sensor_; }

    /**
     * Get the time to sleep after a desired angle is given to the motor.
     *
     * @return the time to sleep after a desired angle is given to the motor.
     */
    std::chrono::milliseconds motorSleepDuration() const { return motor_sleep_; }
    /**
     * Set the time to sleep after a desired angle is given to the motor.
     *
     * @param duration the time to sleep after a desired angle is given to the motor.
     */
    void motorSleepDuration(const std::chrono::milliseconds& duration) { motor_sleep_ = duration; }

    /**
     * Measure the distance to any obstacle.
     *
     * @return An array of measurements associating the angle (in radians) and the distance (in meters).
     */
    std::list<Measure> scan() override;

    /**
     * Start to measure distances.
     *
     * Does nothing if already started.
     */
    void start() override;

    /**
     * Stop to measure distances.
     *
     * Does nothing if if already stopped.
     */
    void stop() override;

private:
    /**
     * Task handling a IR scan.
     *
     * Move the motor clockwise to the max angle.
     * Move the motor counter clockwise to the min angle and measure distances.
     * Store the measurements and repeat.
     */
    void measureTask();
};

#endif // __STOPED_LIDAR_H__
