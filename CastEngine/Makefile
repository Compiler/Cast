CC= g++
INCLUDE=-I include/ -I . 
LIBRARIES =-L lib/ -lglfw -lGL
CFLAGS =-Wall -Wextra -g $(INCLUDE) 


GLAD_ROOT= include/glad_root
main: glad.o
	# Linux compilation 
	g++ $(CFLAGS) -o bin/main glad.o main.cpp $(LIBRARIES)
	./bin/main

glad.o: 
	g++ $(CFLAGS) -c $(GLAD_ROOT)/src/glad.c -o glad.o
