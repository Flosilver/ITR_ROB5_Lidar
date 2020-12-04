#ifndef DCMOTOR_H
#define DCMOTOR_H

//#define MOTOR_ADDR 0x14
#define CMD_BREAK 0x00
#define CMD_STOP 0x01
#define CMD_CW 0x02
#define CMD_CCW 0x03

class DCMotor
{
    public:
        DCMotor(int pin, int i2c_addr);
        virtual ~DCMotor();

        void run_cw(unsigned int speed);
        void run_ccw(unsigned int speed);
        void run(int speed);
        void motor_break();
        void motor_stop();

    private:
        int pin_;
        int i2c_addr_;
        int fd_;
        
};

#endif