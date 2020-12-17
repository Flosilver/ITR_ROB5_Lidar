#ifndef __LIDAR_H__
#define __LIDAR_H__

#include <chrono>
#include <memory>
#include <list>
#include <thread>
#include <mutex>

#include "ir_sensor.h"
#include "dc_motor.h"
#include "shaft_encoder.h"

class Lidar
{
    const std::shared_ptr<DCMotor> pivot_; ///< The pivot that the IR sensor is attached to.
    const std::shared_ptr<ShaftEncoder> encoder_; ///< The encoder
    const std::shared_ptr<IRSensor> sensor_; ///< The IR sensor measuring the distance.
    float min_angle_; ///< The start angle of a measurement (in radians).
    float max_angle_; ///< The end angle of a measurement (in radians).
    float angle_increment_; ///< The increment of the angle between 2 measurements during a scan (in radians).
    std::chrono::milliseconds motor_sleep_; ///< The time to sleep after a desired angle is given to the motor.
    
    bool is_meas_;
    std::mutex meas_mtx_;
    std::thread meas_thread_;

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
          float max_angle = 1.0);
    
    ~Lidar();

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

    std::shared_ptr<const ShaftEncoder> encoder() const { return encoder_; }
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

    struct Measure
    {
        float orientation; ///< Angular position of the measure (in radians).
        float distance; ///< Distance measured between the lidar and the measure (in meters).
    };

    /**
     * Measure the distance to any obstacle.
     *
     * @return An array of measurements associating the angle (in radians) and the distance (in meters).
     */
    std::list<Measure> scan();

    void start();
    void stop();
    void measureTask();

    private:
        std::list<Measure> scan_; ///< zlhgouzhgohgjohrug
};

#endif
