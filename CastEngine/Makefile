CC= g++
INCLUDE=-I include/ -I . -I Cast/ -I include/stb_image/
LIBRARIES=-L lib/ -lglfw -lGL -lbox2d -lfmt
CFLAGS=-Wall -Wextra -g -std=c++20 $(INCLUDE)
EXE_NAME=CastEngine
GLAD_ROOT=include/glad_root
SRC_DIR=Cast
BIN_DIR=bin
OBJECTS=$(BIN_DIR)/glad.o
SRCS:=$(wildcard $(SRC_DIR)/**/*.cpp)
OBJECTS+=$(patsubst $(SRC_DIR)/%.cpp,$(BIN_DIR)/%.o,$(SRCS))

main:$(OBJECTS) $(BIN_DIR)/$(EXE_NAME)
	@echo "Executable size: $(shell ls -lh $(BIN_DIR)/$(EXE_NAME) | awk '{print $$5}')"
	ar crf castengine.a $(OBJECTS)
	./$(BIN_DIR)/$(EXE_NAME)

$(BIN_DIR)/%.o:$(SRC_DIR)/%.cpp
	@echo "Recompiling object files in Cast" $@ $^
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $^

$(BIN_DIR)/glad.o:
	@echo "Recompiling glad" $@ $^
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $(GLAD_ROOT)/src/glad.c -o $(BIN_DIR)/glad.o

$(BIN_DIR)/$(EXE_NAME):$(OBJECTS) main.cpp
	@echo "Recompiling main binary" $@ $^
	$(CC) $(CFLAGS) -g $(OBJECTS) -o $(BIN_DIR)/$(EXE_NAME) main.cpp $(LIBRARIES)

run:
	./$(BIN_DIR)/$(EXE_NAME)

clean:
	rm -rf $(BIN_DIR)/* *.o
	rm Cast/*.o
	rm Cast/Rendering/*.o
	rm Cast/Scenes/*.o
