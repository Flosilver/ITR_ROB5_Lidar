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

protected:
    const std::shared_ptr<IRSensor> sensor_; ///< The IR sensor measuring the distance.
    float min_angle_; ///< The start angle of a measurement (in radians).
    float max_angle_; ///< The end angle of a measurement (in radians).
    float angle_increment_; ///< The increment of the angle between 2 measurements during a scan (in radians).
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
     * @param sensor The IR sensor measuring the distance.
     * @param min_angle The start angle of a measurement (in radians).
     * @param max_angle The end angle of a measurement (in radians).
     */
    Lidar(std::shared_ptr<IRSensor> sensor, float min_angle = -1.0, float max_angle = 1.0);

    Lidar(const Lidar& other) = delete;

    /**
     * Destroy the Lidar.
     *
     * Stop the measurement.
     */
    virtual ~Lidar();

    Lidar& operator=(const Lidar& other) = delete;

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
     * Measure the distance to any obstacle.
     *
     * @return An array of measurements associating the angle (in radians) and the distance (in meters).
     */
    virtual std::list<Measure> scan() = 0;

    /**
     * Start to measure distances.
     *
     * Does nothing if already started.
     */
    virtual void start() = 0;

    /**
     * Stop to measure distances.
     *
     * Does nothing if if already stopped.
     */
    virtual void stop() = 0;
};

#endif // __LIDAR_H__
