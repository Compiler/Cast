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
#include <stb_image/stb_image.h>
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
void initBox2D(){
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.position = (b2Vec2){10.0f, 5.0f};
}
GLFWwindow* window;
int initEngineDeps(){
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



std::vector<float> vertexDataFlat;
std::vector<int> indexDataFlat;
void addRectangle(float x, float y, float width, float height) {
    static std::vector<int> indexTemplate = {0, 1, 3, 1, 2, 3};
    static int vertices = 4;
    int currentOffsetMultiplier = vertexDataFlat.size() / 6;
    // First vertex
    std::vector<float> v1 = {x, y, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
    vertexDataFlat.insert(vertexDataFlat.end(), v1.begin(), v1.end());

    // Second vertex
    std::vector<float> v2 = {x, y - height, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f};
    vertexDataFlat.insert(vertexDataFlat.end(), v2.begin(), v2.end());

    // Third verte
    std::vector<float> v3 = {x - width, y - height, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f};
    vertexDataFlat.insert(vertexDataFlat.end(), v3.begin(), v3.end());

    // Fourth vertex
    std::vector<float> v4 = {x - width, y, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f};
    vertexDataFlat.insert(vertexDataFlat.end(), v4.begin(), v4.end());

    for(int i = 0; i < indexTemplate.size(); i++){
        indexDataFlat.push_back(currentOffsetMultiplier * vertices + indexTemplate[i]);
    }
}

int main() {
   
    float targetFPS = 25;
    auto rand = [](){return 0.3;};//return std::rand() % 255 / 255.0f;};

    if(initEngineDeps() != 0) return -1;

    //space for visual splitting, will move these to own functions / class soon

    stbi_set_flip_vertically_on_load(true);   
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char *data = stbi_load("resources/Assets/grass_jpg.jpg", &width, &height, &nrChannels, 0);
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


    for(float i = -1.0; i < 1.5; i += 0.01)
    addRectangle(i, -0.70, 0.1, 0.1);

    unsigned int vao; glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexDataFlat.size() * sizeof(decltype(vertexDataFlat.front())), vertexDataFlat.data(), GL_STATIC_DRAW);

    unsigned int ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexDataFlat.size() * sizeof(int), indexDataFlat.data(), GL_STATIC_DRAW);
    std::cout << "Element buffer: [";
    for(auto element : indexDataFlat) std::cout << element << ", ";
    std::cout << "]\n";

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(decltype(vertexDataFlat[0])), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(decltype(vertexDataFlat[0])), (void*)(3 * sizeof(decltype(vertexDataFlat[0]))));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(decltype(vertexDataFlat[0])), (void*)(6 * sizeof(decltype(vertexDataFlat[0]))));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);


    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

    std::string vertexShaderCode = readFile("Resources/Shaders/passthrough.vert");
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

    std::string fragmentShaderCode = readFile("Resources/Shaders/passthrough.frag");
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



    glUseProgram(shaderProgram);

    glBindTexture(GL_TEXTURE_2D, texture);



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
        //glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "transform"), 1, GL_FALSE, glm::value_ptr(trans));
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, vertexDataFlat.size(), GL_UNSIGNED_INT, 0);
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

