#pragma once
#include <entt/entity/registry.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#define CHECK_GL_ERROR() Cast::CHECK_FOR_ERROR(__FILE__, __LINE__)
namespace Cast{

    // Global variable
    inline entt::registry ecs_registry;
    extern float frameTimeMs;
    extern float frameTimeMcs;

    inline float window_width = 1920;
    inline float window_height = 1080;

    extern GLFWwindow* window;


    inline void CHECK_FOR_ERROR(const char* file, int line) {
        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR) {
            std::cerr << "OpenGL error (" << file << ":" << line << "): " << err << std::endl;
        }
    }

}
