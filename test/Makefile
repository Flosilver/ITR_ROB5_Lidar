CXX := g++
CXXFLAGS := -g -Wall -std=c++11
INCLUDE := -I../include -I/usr/local/include
LDFLAGS := -L/usr/local/lib
LIBFLAG := -lwiringPi -lpthread

encoder_file:
	sudo mknod -m 666 /tmp/encoder c 239 0

shaft_encoder_test: shaft_encoder_test.cpp ../src/shaft_encoder.cpp
	$(CXX) $^ -o $@ $(INCLUDE) $(CXXFLAGS) $(LDFLAGS) $(LIBFLAG)

motor_ctrl_test: motor_ctrl_test.cpp ../src/motor_ctrl.cpp ../src/shaft_encoder.cpp ../src/dc_motor.cpp
	$(CXX) $^ -o $@ $(INCLUDE) $(CXXFLAGS) $(LDFLAGS) $(LIBFLAG)

lidar_test: lidar_test.cpp ../src/lidar.cpp ../src/motor_ctrl.cpp ../src/shaft_encoder.cpp ../src/dc_motor.cpp ../src/ir_sensor.cpp
	$(CXX) $^ -o $@ $(INCLUDE) $(CXXFLAGS) $(LDFLAGS) $(LIBFLAG)

proximity_tracker_test: proximity_tracker_test.cpp  ../src/proximity_tracker.cpp ../src/lidar.cpp ../src/motor_ctrl.cpp ../src/shaft_encoder.cpp ../src/dc_motor.cpp ../src/ir_sensor.cpp ../src/servo_motor.cpp
	$(CXX) $^ -o $@ $(INCLUDE) $(CXXFLAGS) $(LDFLAGS) $(LIBFLAG)

clear:
	rm shaft_encoder_test motor_ctrl_test lidar_test proximity_tracker_test