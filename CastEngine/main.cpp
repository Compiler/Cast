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



std::vector<float> vertexDataFlat;
std::vector<int> indexDataFlat;
void addRectangle(float x, float y, float width, float height, float textureID) {
    static std::vector<int> indexTemplate = {0, 1, 3, 1, 2, 3};
    static int vertices = 4;
    //int currentOffsetMultiplier = vertexDataFlat.size() / 9;
    static int n = 0;


    std::vector<float> v1 = {x, y, 1.0f,                    1.0f, 1.0f, 1.0f,       0.0f, 1.0f, textureID};
    std::vector<float> v2 = {x + width, y, 1.0f,            1.0f, 1.0f, 1.0f,       1.0f, 1.0f, textureID};
    std::vector<float> v3 = {x + width, y - height, 1.0f,   1.0f, 1.0f, 1.0f,       1.0f, 0.0f, textureID};
    std::vector<float> v4 = {x, y - height, 1.0f,           1.0f, 1.0f, 1.0f,       0.0f, 0.0f, textureID};

    vertexDataFlat.insert(vertexDataFlat.end(), v1.begin(), v1.end());
    vertexDataFlat.insert(vertexDataFlat.end(), v2.begin(), v2.end());
    vertexDataFlat.insert(vertexDataFlat.end(), v3.begin(), v3.end());
    vertexDataFlat.insert(vertexDataFlat.end(), v4.begin(), v4.end());

    // 0, 1, 3, 1, 2, 3
    for(size_t i = 0; i < indexTemplate.size(); i++){
        indexDataFlat.push_back(n * vertices + indexTemplate[i]);
    }
    n++;
}


int main() {
   
    float targetFPS = 25;
    auto rand = [](){return 0.3;};//return std::rand() % 255 / 255.0f;};

    if(initEngineDeps() != 0) return -1;

    //space for visual splitting, will move these to own functions / class soon

    stbi_set_flip_vertically_on_load(true);   
   
glDisable(GL_CULL_FACE);
    float sz = 0.10;
    

    float startingY = -0.75;

    addRectangle(-1, 1, 2, 2, 3);
    addRectangle(-0.5, 0, 0.25, 0.25, 0);
    addRectangle(-0.25, 0, 0.25, 0.25, 1);
    addRectangle(0, 0, 0.25, 0.25, 2);
    addRectangle(0.25, 0, 0.25, 0.25, 3);
    //addRectangle(0, startingY + sz + 1/5.0f, 1/5.0f, 1/5.0f, 3);
    //addRectangle(0, startingY + sz + 1/5.0f, 1/5.0f, 1/5.0f, 3);

    for(float x = -2 ; x <= 2; x += sz) {
        for(float y = startingY; y >= -1.5; y -= sz) {
            addRectangle(x, y, sz, sz, y == startingY ? 1 : 2);
        }
    }


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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(decltype(vertexDataFlat[0])), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(decltype(vertexDataFlat[0])), (void*)(3 * sizeof(decltype(vertexDataFlat[0]))));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(decltype(vertexDataFlat[0])), (void*)(6 * sizeof(decltype(vertexDataFlat[0]))));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    StaticRenderer renderer{};
    Shader myShader{};
    
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

