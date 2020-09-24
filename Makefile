EXE := lidar


SRC_DIR := src
OBJ_DIR := obj
INCLUDE_DIR := include

SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(addprefix $(OBJ_DIR)/, $(notdir $(patsubst %.cpp, %.o, $(SRC))))

CC := g++
INCLUDE := -Iinclude
CFLAGS := -g -Wall -std=c++11
LDFLAGS := 
LIBFLAG := 

.PHONY: all clean mrproper remake rerun


all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $^ -o $@ $(INCLUDE) $(LIBFLAG)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDE) $(LIBFLAG)
	
$(OBJ_DIR):
	mkdir $@

clean:
	rm -rf $(OBJ)

mrproper: clean
	rm -rf $(EXE)

remake: mrproper $(EXE)

rerun: remake
	./$(EXE)