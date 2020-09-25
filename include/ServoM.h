#ifndef SERVOM_H
#define SERVOM_H

#include <wiringPi.h>

class ServoM
{
    protected:
        int _pin;
        float _angle;
    
    private:
        const float& angleToPWM(float angle);

    public:
        ServoM(int nb_pin, float min_duty=3, float max_duty=12);
        ~ServoM();

        const float& getAngle() const;   
        void rotate(float angle);     
};

#endif