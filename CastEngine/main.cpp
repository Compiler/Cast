#include <vector>
#include <math.h>
#include <chrono>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>
#include <thread>
#include <Cast/Core.h>
#include <stb_image/stb_image.h>
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
void initBox2D(){
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.position = (b2Vec2){10.0f, 5.0f};
}

int main() {
    initBox2D();
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

    GLFWwindow* window = glfwCreateWindow(1920, 1080, "Cast", NULL, NULL);
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


    float targetFPS = 25;
    auto rand = [](){return 0.3;};//return std::rand() % 255 / 255.0f;};



    //space for visual splitting, will move these to own functions / class soon





    // x,y correspond to center of triangle
    float x = 0;
    float y = 0;
    float base = 1;
    float height = 1;

    float vertices[] = {
        // first triangle
        0.5f,  0.5f, 0.0f,   1, 0, 0, 
        0.5f, -0.5f, 0.0f,   0, 1, 0, 
        -0.5f, -0.5f, 0.0f,  1, 0, 1, 
        -0.5f,  0.5f, 0.0f,  1, 0, 1 
    };
    unsigned int indices[] = {  
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    std::cout << "Triangle vertices: \n";
    for(int i = 0; i < 6; i++) std::cout << "( " << vertices[i * 3] << ", " << vertices[i*3 + 1] << " ) " << std::endl;

    unsigned int vao; glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(decltype(vertices[0])), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(decltype(vertices[0])), (void*)(3 * sizeof(decltype(vertices[0]))));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);


    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

    std::string vertexShaderCode = readFile("Resources/passthrough.vert");
    const char* sourceCStr = vertexShaderCode.c_str();
    glShaderSource(vertexShader, 1, &sourceCStr, NULL);
    glCompileShader(vertexShader);

    // Check for success
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ShaderError::Vertex::CompilationFailed\t" << infoLog << std::endl;

    }else std::cout << "Shader::Vertex compiled successfully\n";

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    std::string fragmentShaderCode = readFile("Resources/passthrough.frag");
    const char* fragSourceCStr = fragmentShaderCode.c_str();
    glShaderSource(fragmentShader, 1, &fragSourceCStr, NULL);
    glCompileShader(fragmentShader);

    // Check for success
    char infoLogFrag[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLogFrag);
        std::cerr << "ShaderError::Fragment::CompilationFailed\t" << infoLogFrag << std::endl;
    }else std::cout << "Shader::Fragment compiled successfully\n";



    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

     // Check for success
    char infoLogShader[512];
    glGetShaderiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(shaderProgram, 512, NULL, infoLogShader);
        std::cerr << "ShaderError::ShaderProgram::LinkFailed\t" << infoLogShader << std::endl;
    }else std::cout << "Shader::Program compiled successfully\n";






    int frequency = 25;
    float frameLengths;
    float fpss;
    
    int count = 1;
    while (!glfwWindowShouldClose(window)) {
        auto now = std::chrono::high_resolution_clock::now();


        // Pre process 
        processInput(window);

        // Rendering commands
        glClearColor(rand(), rand(), rand(), 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glUniform1f(glGetUniformLocation(shaderProgram, "u_time"), glfwGetTime());
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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

