#ifndef SERVOM_H
#define SERVOM_H

#include <stdio.h>
#include <unistd.h>
#include <iostream.h>
#include <stdlib.h>
//#include <time.h>
//#include <pthread.h>
//#include <semaphore.h>

#include <wiringPi.h>


class ServoM
{
    protected:
        int _pin;
        float _angle;

        float _min;
        float _max;
    
    private:
        const float& angleToPWM(float angle) const;

    public:
        ServoM(int nb_pin, float min_duty=3, float max_duty=12);
        ~ServoM();

        const float& getAngle() const;   
        void rotate(float angle);     
};

#endif