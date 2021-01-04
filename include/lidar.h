#ifndef __LIDAR_H__
#define __LIDAR_H__

#include <atomic>
#include <chrono>
#include <list>
#include <memory>
#include <mutex>
#include <thread>

#include "dc_motor.h"
#include "ir_sensor.h"
#include "shaft_encoder.h"

class Lidar
{
public:
    struct Measure
    {
        float orientation; ///< Angular position of the measure (in radians).
        float distance; ///< Distance measured between the lidar and the measure (in meters).
    };

private:
    const std::shared_ptr<DCMotor> pivot_; ///< The pivot that the IR sensor is attached to.
    const std::shared_ptr<ShaftEncoder> encoder_; ///< The encoder
    const std::shared_ptr<IRSensor> sensor_; ///< The IR sensor measuring the distance.
    const int motor_speed_; ///< The motor rotating speed.
    float min_angle_; ///< The start angle of a measurement (in radians).
    float max_angle_; ///< The end angle of a measurement (in radians).
    float angle_increment_; ///< The increment of the angle between 2 measurements during a scan (in radians).
    std::chrono::milliseconds motor_sleep_; ///< The time to sleep after a desired angle is given to the motor.
    std::chrono::milliseconds motor_stuck_; ///< The duration threshold to consider the motor to be stuck.
    std::chrono::milliseconds unstuck_cooldown_; ///< The cool down before trying to unstuck the motor.
    std::atomic<bool> is_measuring_; ///< The flag controlling the infinite loop in the thread.
    std::mutex meas_mtx_; ///< The mutex used to synchronize operations around the measurements.
    std::thread meas_thread_; ///< The thread that performs the measurements.
    std::list<Measure> scan_; ///< The last measurements from the thread.

public:
    /**
     * Normalize an angle.
     *
     * @param angle The given angle (in radians).
     * @return The angle in [-pi, pi[ (in radians).
     */
    static float normalize(float angle);

    /**
     * Construct a new dummy Lidar.
     */
    Lidar() = default;
    /**
     * Construct a new Lidar.
     *
     * @param pivot The pivot that the IR sensor is attached to.
     * @param sensor The IR sensor measuring the distance.
     * @param min_angle The start angle of a measurement (in radians).
     * @param max_angle The end angle of a measurement (in radians).
     */
    Lidar(std::shared_ptr<DCMotor> pivot,
          std::shared_ptr<ShaftEncoder> encoder,
          std::shared_ptr<IRSensor> sensor,
          float min_angle = -1.0,
          float max_angle = 1.0,
          int motor_speed = 100);

    Lidar(const Lidar& other) = delete;

    /**
     * Destroy the Lidar.
     *
     * Stop the measurement.
     */
    ~Lidar();

    Lidar& operator=(const Lidar& other) = delete;

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
     * Get the start angle of a measurement (in radians).
     *
     * @return the start angle of a measurement (in radians).
     */
    float minAngle() const { return min_angle_; }
    /**
     * Set the start angle of a measurement (in radians).
     *
     * @param angle the start angle of a measurement (in radians).
     */
    void minAngle(float angle);

    /**
     * Get the end angle of a measurement (in radians).
     *
     * @return the end angle of a measurement (in radians).
     */
    float maxAngle() const { return max_angle_; }
    /**
     * Set the end angle of a measurement (in radians).
     *
     * @param angle the end angle of a measurement (in radians).
     */
    void maxAngle(float angle);

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
    std::list<Measure> scan();

    /**
     * Start to measure distances.
     *
     * Does nothing if already started.
     */
    void start();

    /**
     * Stop to measure distances.
     *
     * Does nothing if if already stopped.
     */
    void stop();

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

#endif // __LIDAR_H__
