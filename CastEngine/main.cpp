#define STB_IMAGE_IMPLEMENTATION

#include <vector>

#include <math.h>
#include <chrono>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>
#include <thread>
#include <Cast/Core.h>
#include <Cast/Rendering/Shader.h>
#include <Cast/Rendering/StaticRenderer.h>
#include <box2d/box2d.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#ifdef CAST_MAC_OS
#define OPENGL_MINOR_VERSION 1
#else 
#ifdef CAST_WIN_OS
#define OPENGL_MINOR_VERSION 6
#else
#define OPENGL_MINOR_VERSION 2
#endif
#endif


#include <fstream>
#include <sstream>
#include <string>

std::string readFile(std::string filePath){
    std::ifstream file(filePath);
    if(!file){
        std::cerr << "Failed to load file " << filePath << std::endl;
        return "";
    }
    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

void framebuffer_size_callback(GLFWwindow* , int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void glfwErrorCallback(int error, const char* description) {
    std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
}
GLFWwindow* window;
int initEngineDeps(){
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

     window = glfwCreateWindow(1920, 1080, "Cast", NULL, NULL);
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

    return 0;

}



int main() {
   
    float targetFPS = 25;
    auto rand = [](){return 0.3;};//return std::rand() % 255 / 255.0f;};

    if(initEngineDeps() != 0) return -1;

    //space for visual splitting, will move these to own functions / class soon

    stbi_set_flip_vertically_on_load(true);   
   
    glDisable(GL_CULL_FACE);
    
    StaticRenderer renderer{};
    Shader myShader{};    
    renderer.addRectangle(-1, 1, 2, 2, 3);
    renderer.addRectangle(-0.5, 0, 0.25, 0.25, 0);
    renderer.addRectangle(-0.25, 0, 0.25, 0.25, 1);
    renderer.addRectangle(0, 0, 0.25, 0.25, 2);
    renderer.addRectangle(0.25, 0, 0.25, 0.25, 3);

    float sz = 0.10;
    float startingY = -0.75;
    for(float x = -2 ; x <= 2; x += sz) {
        for(float y = startingY; y >= -1.5; y -= sz) {
            renderer.addRectangle(x, y, sz, sz, y == startingY ? 1 : 2);
        }
    }

    myShader.addShader(GL_VERTEX_SHADER, "Resources/Shaders/passthrough.vert");
    myShader.addShader(GL_FRAGMENT_SHADER, "Resources/Shaders/passthrough.frag");
    myShader.compile();

    unsigned int shaderProgram = myShader.getUID();
    

    glUseProgram(shaderProgram);

    renderer.addTexture("Resources/Assets/player.png");
    renderer.addTexture("Resources/Assets/grass_jpg.jpg");
    renderer.addTexture("Resources/Assets/dirt.png");
    renderer.addTexture("Resources/Assets/landscape_mountains.png");


    glUniform1i(glGetUniformLocation(shaderProgram, "u_texture1"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "u_texture2"), 1);
    glUniform1i(glGetUniformLocation(shaderProgram, "u_texture3"), 2);
    glUniform1i(glGetUniformLocation(shaderProgram, "u_texture4"), 3);
    


    int frequency = 25;
    float frameLengths;
    float fpss;
    
    int count = 1;
    renderer.preDraw();
    while (!glfwWindowShouldClose(window)) {
        auto now = std::chrono::high_resolution_clock::now();


        // Pre process 
        processInput(window);

        // Rendering commands
        glClearColor(rand(), rand(), rand(), 1);
        glClear(GL_COLOR_BUFFER_BIT);


        glUseProgram(shaderProgram);
        glUniform1f(glGetUniformLocation(shaderProgram, "u_time"), glfwGetTime());
        //glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "transform"), 1, GL_FALSE, glm::value_ptr(trans));
        renderer.draw();
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        // Check and proc events, swap render buffers
        glfwSwapBuffers(window);
        glfwPollEvents();



        auto delta = std::chrono::high_resolution_clock::now() - now;
        float frameTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(delta).count() ;
        float fps = 1000.0f / frameTimeMs;
        frameLengths += frameTimeMs;
        fpss += fps;
        count ++;
        if(count == frequency){
            std::cout << "Frame length: " << frameLengths / frequency << "(" << frameTimeMs << ") " << "ms" << std::endl;
            std::cout << "FPS: " << std::round(fpss/frequency) << "(" << fps << ")" << std::endl;
            count = 1;

            frameLengths = 0;
            fpss = 0;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds((int)(1000 / targetFPS - frameTimeMs)));
    }

    glfwTerminate();
    return 0;
}

