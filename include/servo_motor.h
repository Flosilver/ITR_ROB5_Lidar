#ifndef SERVOM_H
#define SERVOM_H

class ServoMotor
{

public:
    ServoMotor(int nb_pin, float min_duty = 3.0F, float max_duty = 12.0F);
    ~ServoMotor();

    float angle() const;
    void rotate(float angle);

private:
    float angleToPWM(float angle) const;

    int pin_;
    float angle_;
    int min_;
    int delta_;
};

#endif
