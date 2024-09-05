CC= g++
INCLUDE=-I include/ -I . 

# Linux compilation 
LIBRARIES =-L lib/ -lglfw -lGL
CFLAGS =-Wall -Wextra -g $(INCLUDE) 

EXE_NAME=CastEngine

GLAD_ROOT=include/glad_root
BIN_DIR=bin

OBJECTS=$(BIN_DIR)/glad.o Cast/Core.o

$(BIN_DIR)/$(EXE_NAME): $(OBJECTS) main.cpp run

%.o: %.cpp
	@echo "Recompiling object files in Cast " $@ " " $^
	$(CC) $(CFLAGS) -c -o $@ $^

$(BIN_DIR)/glad.o: 
	@echo "Recompiling glad " $@ " " $^
	$(CC) $(CFLAGS) -c $(GLAD_ROOT)/src/glad.c -o $(BIN_DIR)/glad.o

main.cpp:
	@echo "Recompiling main binary " $@ ": " $^
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$(EXE_NAME) $^ $(LIBRARIES)

run:
	./$(BIN_DIR)/$(EXE_NAME)

clean:
	rm -rf $(BIN_DIR)/* *.o
