#ifndef __LIDAR_CONTINUOUS_H__
#define __LIDAR_CONTINUOUS_H__

#include <atomic>
#include <chrono>
#include <list>
#include <memory>
#include <mutex>
#include <thread>
#include "dc_motor.h"
#include "ir_sensor.h"
#include "lidar.h"
#include "shaft_encoder.h"

class LidarContinuous : public Lidar
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

public:
    /**
     * Construct a new dummy LidarContinuous.
     */
    LidarContinuous() = default;
    /**
     * Construct a new LidarContinuous.
     *
     * @param pivot The pivot that the IR sensor is attached to.
     * @param sensor The IR sensor measuring the distance.
     * @param min_angle The start angle of a measurement (in radians).
     * @param max_angle The end angle of a measurement (in radians).
     * @param motor_speed The speed of the motor.
     * @param motor_sleep Sleep duration between motor position checks (in milli seconds).
     * @param motor_stuck Duration after which the motor is considered stuck (in milli seconds).
     * @param motor_cool Cool down duration between trial of unstucking the motor (in milli seconds).
     */
    LidarContinuous(std::shared_ptr<DCMotor> pivot,
                    std::shared_ptr<ShaftEncoder> encoder,
                    std::shared_ptr<IRSensor> sensor,
                    float min_angle = -1.0,
                    float max_angle = 1.0,
                    unsigned int motor_speed = 100,
                    unsigned int motor_sleep = 20,
                    unsigned int motor_stuck = 100,
                    unsigned int motor_cool = 500);

    LidarContinuous(const LidarContinuous& other) = delete;

    /**
     * Destroy the LidarContinuous.
     *
     * Stop the measurement.
     */
    ~LidarContinuous();

    LidarContinuous& operator=(const LidarContinuous& other) = delete;

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

#endif // __LIDAR_CONTINUOUS_H__
