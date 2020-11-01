#ifndef SERVOM_H
#define SERVOM_H

class ServoMotor
{

public:
    ServoMotor(int nb_pin, int min_duty = 5, int max_duty = 22);
    ~ServoMotor();

    float angle() const;
    void rotate(float angle);

private:
    int pin_;
    float angle_;
    int min_;
    int delta_;
};

#endif
