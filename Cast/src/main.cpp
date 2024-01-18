
#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <iostream>
#include <random>

int main(void){

    

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    int width = 2560;
    int height = static_cast<double>(width) * 9.0 / 16.0;
    std::cout << "Dim: " << width << "x" << height << std::endl;
    GLFWwindow* window = glfwCreateWindow(width, height, "Cast", NULL, NULL);
    if (window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }   

    glViewport(0, 0, width, height);
    int count = 0;
    while(!glfwWindowShouldClose(window)){
        glfwSwapBuffers(window);
        glfwPollEvents();    
        glClearColor((float)std::rand()/RAND_MAX, (float)std::rand()/RAND_MAX, (float)std::rand()/RAND_MAX, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    return 0;
}