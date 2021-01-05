#ifndef SERVOM_H
#define SERVOM_H

class ServoMotor
{
    int pin_; ///< The physical pin.
    float angle_; ///< The shaft orientation (in radians).
    int min_; ///< The minimal duty cycle (in percents).
    int delta_; ///< The variation range of the duty cycle (in percents).

public:
    ServoMotor() = default;
    /**
     * Construct a new Servo Motor.
     *
     * @param pin The physical pin.
     * @param min_duty The minimal duty cycle (in percents).
     * @param max_duty The maximal duty cycle (in percents).
     */
    ServoMotor(int pin, int min_duty = 5, int max_duty = 21);
    ServoMotor(const ServoMotor& other) = delete;
    ~ServoMotor() {}

    ServoMotor& operator=(const ServoMotor& other) = delete;

    /**
     * Get the minimal duty cycle.
     *
     * @return The minimal duty cycle (in percents).
     */
    int minDuty() const { return min_; }

    /**
     * Set the minimal duty cycle.
     *
     * @param val The minimal duty cycle (in percents).
     */
    void minDuty(int val) { min_ = val; }

    /**
     * Get the maximal duty cycle.
     *
     * @return The maximal duty cycle (in percents).
     */
    int maxDuty() const { return min_ + delta_; }

    /**
     * Set the maximal duty cycle.
     *
     * @param val the maximal duty cycle (in percents).
     */
    void maxDuty(int val) { delta_ = val - min_; }

    /**
     * Get the last desired angle.
     *
     * @return the last desired angle (in radians).
     */
    float angle() const;

    /**
     * Rotate the shaft to the given angle in [-pi/2,pi/2].
     *
     * @param angle the desired angle (in radians).
     */
    void rotate(float angle);
};

#endif
