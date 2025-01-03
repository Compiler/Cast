#pragma once
#include <ostream>
#define GLFW_INCLUDE_NONE
#define GLM_ENABLE_EXPERIMENTAL

#include <Cast/Cast.h>
#include <Cast/Common.h>

#include <math.h>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
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
#define OPENGL_MINOR_VERSION 6
#endif
#endif

#include <iostream>
#include <entt/entt.hpp>
#include <Cast/ECS/BasicComponents.h>
#include <Cast/ECS/BasicSystems.h>
#include <Cast/Scenes/DebugScene.h>

using namespace Cast;

class Core{
    private:
        DebugScene _debugScene;

        static void glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam); 
        static inline void framebuffer_size_callback(GLFWwindow* , int width, int height) {
            glViewport(0, 0, width, height);
            window_width = width;
            window_height = height;
        }

        static inline void processInput(GLFWwindow* window) {
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(window, true);
            if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
                glfwSetWindowShouldClose(window, true);
        }

        static float myX;
        static float myY;
        static inline void glfwErrorCallback(int error, const char* description) {
            std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
        }
        int _initEngineDependencies();
        void generateEntity(float x, float y, float id, std::string name);
    public:

        Core() {
            std::cout << "Created core object.";
        }
        int init();
        void update();
        void render();
        int shutdown(){ glfwTerminate(); return 0;}

        void setDelta(float& d, float& mcs){ 
            Cast::frameTimeMs = mcs/1000.0;
            Cast::frameTimeMcs = mcs;
        }
        inline GLFWwindow* getWindow() const { return Cast::window; }
        inline bool shouldClose() const { return glfwWindowShouldClose(Cast::window); }
};
