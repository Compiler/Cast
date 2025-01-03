#include "Core.h"
#include "Common.h"
#include "ECS/BasicComponents.h"
#include "ECS/BasicSystems.h"
#include "Rendering/DynamicRenderer.h"
#include "Rendering/StaticRenderer.h"
#include <bitset>
#include <fmt/core.h>
#include <ostream>
#include <thread>

float Core::myX = 0;
float Core::myY = 0;
float Cast::frameTimeMs = 0;
float Cast::frameTimeMcs = 0;
GLFWwindow* Cast::window = nullptr;
void Core::glDebugOutput(GLenum source, GLenum type, GLuint id,
                                  GLenum severity, GLsizei length,
                                  const GLchar* message, const void* userParam) {
    std::cerr << "OpenGL Debug Message:\n";
    std::cerr << "Source: " << source << ", Type: " << type << ", ID: " << id << "\n";
    std::cerr << "Severity: " << severity << "\n";
    std::cerr << message << "\n";
}


int Core::init(){
    if(_initEngineDependencies() != 0) return -1;
    CAST_LOG("{}", "Init\n");
   
    CHECK_GL_ERROR();
    _debugScene.init();
    CHECK_GL_ERROR();
    return 0;
}


void Core::update(){
    CHECK_GL_ERROR();
    _debugScene.update(frameTimeMs);
}
void Core::render(){
    CHECK_GL_ERROR();
    // Pre process 
    processInput(Cast::window);

    // Rendering commands
    glClearColor(0.25, 0.25, 0.5, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    _debugScene.render(frameTimeMs);


    // Check and proc events, swap render buffers
    glfwSwapBuffers(Cast::window);
    glfwPollEvents();
}



int Core::_initEngineDependencies(){

    glfwSetErrorCallback(glfwErrorCallback);
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_MINOR_VERSION);

    std::cout << "Done initializing GLFW\n";
    #ifdef CAST_MAC_OS 
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    Cast::window = glfwCreateWindow(window_width, window_height, "Cast", NULL, NULL);
    if (!Cast::window) {
        std::cerr << "Failed to create GLFW Cast::window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(Cast::window);

    std::cout << "Done creating window\n";
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glfwSetFramebufferSizeCallback(Cast::window, framebuffer_size_callback);

    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);

    if (!renderer || !version) {
        std::cerr << "Failed to retrieve OpenGL context information." << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL Version: " << version << std::endl;

    auto err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cout << "OpenGL error after glDisable(GL_CULL_FACE): " << err << std::endl;
    }
    if (GLAD_GL_VERSION_4_3) { // Debug output is supported in OpenGL 4.3 and above
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(Core::glDebugOutput, nullptr);
    } else {
        std::cerr << "GL_DEBUG_OUTPUT not supported on this OpenGL version." << std::endl;
    }

    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glfwSwapInterval(0); // Disable vsync


    return 0;
}
