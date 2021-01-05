#include <iostream>
#include <memory>
#include "motor_ctrl.h"
#include "shaft_encoder.h"

int main(void)
{
    std::shared_ptr<ShaftEncoder> encoder(new ShaftEncoder("/tmp/encoder", 420));
    encoder->offset(encoder->measureIncrements());
    MotorCtrl motor(0, 0x14, encoder);

    std::cout << "Press Enter to continue...";
    std::cin.get();

    motor.rotateAsync(0.6);

    std::cout << "Press Enter to continue...";
    std::cin.get();

    motor.rotateAsync(0);

    std::cout << "Press Enter to exit...";
    std::cin.get();
    return 0;
}
