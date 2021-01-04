#include <iostream>
#include <wiringPi.h>
#include <memory>
#include "gmft_game.h"

int main()
{
    wiringPiSetup(); // setup the library
    std::cout << "wiring PI seted up\n";

    std::shared_ptr<ShaftEncoder> encoder(new ShaftEncoder("/tmp/encoder", 420));
    encoder->setOffset(encoder->measureIncrements());

    std::shared_ptr<DCMotor> motor(new DCMotor(0, 0x14));

    std::shared_ptr<IRSensor> sensor(new IRSensor(4, 1));

    std::shared_ptr<Lidar> lidar(new Lidar(motor, encoder, sensor, -0.4, 0.4));

    std::shared_ptr<Referee> ref(new Referee(0, 22, 0, 0.5, 25., 3000));

    GMFTGame gmft(lidar, ref, 0.4, 0.1, 0.03);

    std::cout << "Starting the tracker..." << std::endl;
    gmft.play();

    // Terminer le main
    std::cout << "Press Enter to exit...";
    std::cin.get();

    return 0;
}