CAST_LIB = Cast
EXTERN_LIB = $(CAST_LIB)/lib

GLFW = $(EXTERN_LIB)/GLFW
GLFW_LIB = $(GLFW)/lib
GLFW_INC = $(GLFW)/include

GLAD = $(EXTERN_LIB)/GLAD
GLAD_INC = $(GLAD)/include

CLIENT_DRIVER = Cast/src/main.cpp

CPP = g++
CPP_FLAGS = -std=c++17

INCLUDES = -I $(GLAD_INC) -I $(CAST_LIB) -I $(GLFW_INC)
LIBRARIES = -L$(GLFW_LIB) -lglfw3 -lgdi32 -luser32 -lkernel32 -lopengl32

BIN = bin
OUT = programBinary

main:
	$(CPP) $(CPP_FLAGS) $(INCLUDES) -c $(GLAD_INC)/glad/glad.c -o $(BIN)/glad.o
	$(CPP) $(CPP_FLAGS) $(INCLUDES) -c $(CLIENT_DRIVER) -o $(BIN)/$(OUT).o
	$(CPP) $(CPP_FLAGS) $(BIN)/$(OUT).o $(BIN)/glad.o $(LIBRARIES) -o $(BIN)/$(OUT)
	./$(BIN)/$(OUT)
