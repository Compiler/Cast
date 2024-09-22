CC= g++
INCLUDE=-I include/ -I . -I Cast/ -I include/stb_image/ 

# Linux compilation 
LIBRARIES =-L lib/ -lglfw -lGL -lbox2d
CFLAGS =-Wall -Wextra -g -std=c++20 $(INCLUDE) 

EXE_NAME=CastEngine

GLAD_ROOT=include/glad_root
BIN_DIR=bin

OBJECTS=$(BIN_DIR)/glad.o Cast/Core.o Cast/Rendering/DynamicRenderer.o Cast/Rendering/StaticRenderer.o Cast/Rendering/shader.o

main: $(OBJECTS) $(BIN_DIR)/$(EXE_NAME)
	@echo "Executable size: $(shell ls -lh $(BIN_DIR)/$(EXE_NAME) | awk '{print $$5}')"
	ar crf castengine.a $(OBJECTS)
	./$(BIN_DIR)/$(EXE_NAME)


%.o: %.cpp
	@echo "Recompiling object files in Cast " $@ " " $^
	$(CC) $(CFLAGS) -c -o $@ $^

$(BIN_DIR)/glad.o: 
	@echo "Recompiling glad " $@ " " $^
	$(CC) $(CFLAGS) -c $(GLAD_ROOT)/src/glad.c -o $(BIN_DIR)/glad.o

$(BIN_DIR)/$(EXE_NAME): $(OBJECTS) main.cpp
	@echo "Recompiling main binary " $@ ": " $^
	$(CC) $(CFLAGS) -g $(OBJECTS) -o $(BIN_DIR)/$(EXE_NAME) main.cpp $(LIBRARIES)

run:
	./$(BIN_DIR)/$(EXE_NAME)

clean:
	rm -rf $(BIN_DIR)/* *.o
	rm Cast/*.o
	rm Cast/Rendering/*.o
