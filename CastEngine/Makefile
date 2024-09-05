CC= g++
INCLUDE=-I include/ -I . 
LIBRARIES =-L lib/ -lglfw -lGL
CFLAGS =-Wall -Wextra -g $(INCLUDE) 

EXE_NAME=CastEngine

GLAD_ROOT= include/glad_root
BIN_DIR=bin

OBJECTS = Cast/Core.o

main: $(BIN_DIR)/glad.o $(OBJECTS)
	# Linux compilation 
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$(EXE_NAME) $^ main.cpp $(LIBRARIES)
	./$(BIN_DIR)/$(EXE_NAME)


%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $^

$(BIN_DIR)/glad.o: 
	$(CC) $(CFLAGS) -c $(GLAD_ROOT)/src/glad.c -o $(BIN_DIR)/glad.o


clean:
	rm -rf $(BIN_DIR)/* *.o
