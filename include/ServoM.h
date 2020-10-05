#ifndef SERVOM_H
#define SERVOM_H

#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
//#include <time.h>
//#include <pthread.h>
//#include <semaphore.h>

#include <wiringPi.h>


class ServoM
{
    protected:
        int pin_;
        float angle_;

        float min_;
        float max_;
    
    private:
        float angleToPWM(float angle) const;

    public:
        ServoM(int nb_pin, float min_duty=3, float max_duty=12);
        ~ServoM();

        const float& getAngle() const;   
        void rotate(float angle);     
};

#endif