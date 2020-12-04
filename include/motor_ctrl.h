#ifndef __MOTOR_CTRL_H__
#define __MOTOR_CTRL_H__

#include "motor_dc.h"
#include "shaft_encoder.h"

#include <iostream>
#include <thread>
#include <memory>
#include <mutex>
#include <string>

class MotorCtrl : public DCMotor {
    public:
        MotorCtrl(int pin, int i2c_addr, std::string encoder_path, long encoder_cpr, float kp = 1, float ki = 1);
        virtual ~MotorCtrl();

        void rotate(float val);

    private:
        ShaftEncoder encoder_;
        std::thread ctrl_thread_;
        std::mutex input_mtx_;
        float input_;
        float kp_;
        float ki_;
        float err_int_;
        bool bool_;

        void control();
};

#endif