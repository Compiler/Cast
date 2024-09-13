#include <chrono>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>
#include <thread>
#include <Cast/Core.h>

#ifdef CAST_MAC_OS
#define OPENGL_MINOR_VERSION 1
#else 
#ifdef CAST_WIN_OS
#define OPENGL_MINOR_VERSION 6
#else
#define OPENGL_MINOR_VERSION 2
#endif
#endif

void framebuffer_size_callback(GLFWwindow* , int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void glfwErrorCallback(int error, const char* description) {
    std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
}

int main() {
    glfwSetErrorCallback(glfwErrorCallback);
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_MINOR_VERSION);

#ifdef CAST_MAC_OS
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    Core().init();


    GLFWwindow* window = glfwCreateWindow(800, 600, "Cast", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glViewport(0, 0, 1920, 1080);
    float targetFPS = 10;
    while (!glfwWindowShouldClose(window)) {
        auto now = std::chrono::high_resolution_clock::now();
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT);

        auto rand = [](){return std::rand() % 255 / 255.0f;};
        glClearColor(rand(), rand(), rand(), 1);

        glfwSwapBuffers(window);

        glfwPollEvents();
        auto delta = std::chrono::high_resolution_clock::now() - now;
        float frameTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(delta).count() ;
            //22ms = 0.022
        float fps = 1000.0f / frameTimeMs;
        std::cout << "FPS: " << fps << "\n Frame time: " << frameTimeMs << std::endl;
        if(fps > 100) std::this_thread::sleep_for(std::chrono::milliseconds((int)(1000 / targetFPS - frameTimeMs)));
    }

    glfwTerminate();
    return 0;
}

