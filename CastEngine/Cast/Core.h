#pragma once
#define GLFW_INCLUDE_NONE

#include <math.h>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>
#include <Cast/Core.h>
#include <Cast/Rendering/Shader.h>
#include <Cast/Rendering/StaticRenderer.h>
#include <Cast/Rendering/DynamicRenderer.h>
#include <box2d/box2d.h>

#ifdef CAST_MAC_OS
#define OPENGL_MINOR_VERSION 1
#else 
#ifdef CAST_WIN_OS
#define OPENGL_MINOR_VERSION 6
#else
#define OPENGL_MINOR_VERSION 2
#endif
#endif

#include <iostream>
#include <entt/entt.hpp>
#include <Cast/ECS/BasicComponents.h>

using namespace Cast;

class Core{
    private:
        GLFWwindow* _window;
        static inline void framebuffer_size_callback(GLFWwindow* , int width, int height) {
            glViewport(0, 0, width, height);
        }

        static inline void processInput(GLFWwindow* window) {
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(window, true);
            if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
                glfwSetWindowShouldClose(window, true);

        }

        static inline void glfwErrorCallback(int error, const char* description) {
            std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
        }
        StaticRenderer renderer;
        DynamicRenderer rend;
        Shader myShader;    
    public:
        Core() {
            std::cout << "Created core object.";
        }
        int init();
        void update();
        void render();
        int shutdown(){ glfwTerminate(); return 0;}

        inline GLFWwindow* getWindow() const { return _window; }
        inline bool shouldClose() const { return glfwWindowShouldClose(_window); }
};
