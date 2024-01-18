
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

INCLUDES = -I $(CAST_LIB) -I $(GLFW_INC) -I $(GLAD_INC)/
LIBRARIES = -L$(GLFW_LIB) -lglfw3 -lgdi32 -luser32 -lkernel32

BIN = bin
OUT = programBinary

main:
	$(CPP) $(CPP_FLAGS) $(INCLUDES) $(LIBRARIES) $(CLIENT_DRIVER) -o $(BIN)/$(OUT)
	./$(BIN)/$(OUT)