EXE := itr-project


SRC_DIR := src
OBJ_DIR := obj
INCLUDE_DIR := include

SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(addprefix $(OBJ_DIR)/, $(notdir $(patsubst %.cpp, %.o, $(SRC))))

CXX := g++
INCLUDE := -Iinclude -I/usr/local/include
CFLAGS := -g -Wall -std=c++11 `pkg-config --cflags opencv`
LDFLAGS := -L/usr/local/lib
LIBFLAG := -lwiringPi -lpthread -lwiringPi -lpthread `pkg-config --libs opencv`

.PHONY: all clean mrproper remake rerun


all: $(EXE)

$(EXE): $(OBJ)
	$(CXX) $^ -o $@ $(INCLUDE) $(LDFLAGS) $(LIBFLAG)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) -c $< -o $@ $(CFLAGS) $(INCLUDE) $(LIBFLAG)

$(OBJ_DIR):
	mkdir $@

clean:
	rm -rf $(OBJ)

mrproper: clean
	rm -rf $(EXE)

remake: mrproper $(EXE)

rerun: remake
	./$(EXE)
