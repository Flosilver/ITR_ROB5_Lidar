#ifndef __LIDAR_STEP_H__
#define __LIDAR_STEP_H__

#include <chrono>
#include <list>
#include <memory>

#include "ir_sensor.h"
#include "lidar.h"
#include "motor_ctrl.h"

class LidarStep : public Lidar
{
    const std::shared_ptr<MotorCtrl> pivot_; ///< The pivot that the IR sensor is attached to.
    std::chrono::milliseconds motor_sleep_; ///< The time to sleep after a desired angle is given to the motor.

public:
    /**
     * Construct a new dummy LidarStep.
     */
    LidarStep() = default;
    /**
     * Construct a new LidarStep.
     *
     * @param pivot The pivot that the IR sensor is attached to.
     * @param sensor The IR sensor measuring the distance.
     * @param min_angle The start angle of a measurement (in radians).
     * @param max_angle The end angle of a measurement (in radians).
     */
    LidarStep(std::shared_ptr<MotorCtrl> pivot,
              std::shared_ptr<IRSensor> sensor,
              float min_angle = -1.0,
              float max_angle = 1.0);

    /**
     * Get the pivot that the IR sensor is attached to.
     *
     * @return the pivot that the IR sensor is attached to.
     */
    std::shared_ptr<const MotorCtrl> pivot() const { return pivot_; }
    /**
     * @overload pivot()
     */
    std::shared_ptr<MotorCtrl> pivot() { return pivot_; }

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
    virtual void start() override {}

    /**
     * Stop to measure distances.
     *
     * Does nothing if if already stopped.
     */
    virtual void stop() override {}
};
#endif // __LIDAR_STEP_H__
