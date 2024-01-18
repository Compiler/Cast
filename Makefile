CAST_LIB = Cast
EXTERN_LIB = $(CAST_LIB)/lib

GLFW = $(EXTERN_LIB)/GLFW
GLFW_LIB = $(GLFW)/lib
GLFW_INC = $(GLFW)/include

GLAD = $(EXTERN_LIB)/GLAD
GLAD_INC = $(GLAD)/include

CLIENT_DRIVER = Cast/src/main.cpp

CPP = g++
CPP_FLAGS = -std=c++20

INCLUDES = -I $(GLAD_INC) -I $(CAST_LIB) -I $(GLFW_INC)
LIBRARIES = -L$(GLFW_LIB) -lglfw3 -lgdi32 -luser32 -lkernel32 -lopengl32

BIN = bin
OUT = programBinary

GLAD_OBJ = $(BIN)/glad.o
CLIENT_OBJ = $(BIN)/$(OUT).o

.PHONY: main

main: $(GLAD_OBJ) $(CLIENT_OBJ)
	$(CPP) $(CPP_FLAGS) $(GLAD_OBJ) $(CLIENT_OBJ) $(LIBRARIES) -o $(BIN)/$(OUT)
	./$(BIN)/$(OUT)

$(GLAD_OBJ): $(GLAD_INC)/glad/glad.c
	$(CPP) $(CPP_FLAGS) $(INCLUDES) -c $< -o $@

$(CLIENT_OBJ): $(CLIENT_DRIVER)
	$(CPP) $(CPP_FLAGS) $(INCLUDES) -c $< -o $@
