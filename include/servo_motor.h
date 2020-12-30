#ifndef SERVOM_H
#define SERVOM_H

class ServoMotor
{
    int pin_;
    float angle_;
    int min_;
    int delta_;

public:
    ServoMotor(int nb_pin, int min_duty = 5, int max_duty = 21);
    ~ServoMotor();

    int minDuty() const { return min_; }
    void minDuty(int val) { min_ = val; }

    int maxDuty() const { return min_ + delta_; }
    void maxDuty(int val) { delta_ = val - min_; }

    float angle() const;
    void rotate(float angle);
};

#endif
