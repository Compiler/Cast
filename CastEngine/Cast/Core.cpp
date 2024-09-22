#include "Core.h"
#include "Common.h"
#include "ECS/BasicComponents.h"
#include "Rendering/DynamicRenderer.h"
#include "Rendering/StaticRenderer.h"
#include <algorithm>
#include <chrono>

float Core::myX = 900;
float Core::myY = 900;
float Cast::frameTimeMs = 0;
int Core::init(){
   
    renderer = new StaticRenderer();
    CHECK_GL_ERROR();
    dyRenderer = new DynamicRenderer();
    CHECK_GL_ERROR();
    myShader = new Shader();
    CHECK_GL_ERROR();
    
    int count = 0;

    renderer->addRectangle(0, 1080, 1980, 1080, 3);

    float sz = 50;
    float startingY = 300;
    for(float x = 0 ; x <= 1980; x += sz) {
        for(float y = startingY; y >= sz; y -= sz) {
            renderer->addRectangle(x, y, sz, sz, y == startingY ? 1 : 2);
        }
    }

    renderer->addRectangle(0, 400, 100, 100, (count++) % 4);
    renderer->addRectangle(0, 300, 100, 100, (count++) % 4);
    renderer->addRectangle(0, 200, 100, 100, (count++) % 4);
    renderer->addRectangle(0, 100, 100, 100, (count++) % 4);
    //ENTT

    auto entity = ecs_registry.create();

    auto transform = ecs_registry.emplace<Transform>(entity);
    ecs_registry.emplace<Renderable>(entity);
    
    transform.position.x = 500;
    transform.position.y = 500;

    for(auto&& [entity, trans, rend] : ecs_registry.view<Transform, Renderable>().each()){
        renderer->addRectangle(trans.position.x, trans.position.y, 50, 50, -1);
    }
    myShader->addShader(GL_VERTEX_SHADER, "Resources/Shaders/passthrough.vert");
    myShader->addShader(GL_FRAGMENT_SHADER, "Resources/Shaders/passthrough.frag");
    myShader->compile();

    glUseProgram(myShader->getUID());

    renderer->addTexture("Resources/Assets/truffle_spritesheet.png");
    renderer->addTexture("Resources/Assets/grass_jpg.jpg");
    renderer->addTexture("Resources/Assets/dirt.png");
    renderer->addTexture("Resources/Assets/landscape_mountains.png");

    glUniform1i(glGetUniformLocation(myShader->getUID(), "u_texture1"), 0);
    glUniform1i(glGetUniformLocation(myShader->getUID(), "u_texture2"), 1);
    glUniform1i(glGetUniformLocation(myShader->getUID(), "u_texture3"), 2);
    glUniform1i(glGetUniformLocation(myShader->getUID(), "u_texture4"), 3);

     renderer->preDraw();
    CHECK_GL_ERROR();

    return 0;

}


void Core::update(){

    myY -= 0.00998 / frameTimeMs;
    if(myY < 300 + 50) myY = 300 + 50;
}
void Core::render(){
    CHECK_GL_ERROR();
    // Pre process 
    processInput(_window);

    // Rendering commands
    glClearColor(0.7, 0.5, 0.8, 1);
    glClear(GL_COLOR_BUFFER_BIT);


    glUseProgram(myShader->getUID());
    auto proj = glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f, -1.0f, 5.0f);
    glUniformMatrix4fv(glGetUniformLocation(myShader->getUID(), "u_projection"),1, GL_FALSE, glm::value_ptr(proj));


    glUniform1f(glGetUniformLocation(myShader->getUID(), "u_time"), glfwGetTime());

    renderer->draw();

    // static
    static bool oneTime = true;
    static unsigned int _vao, _vbo, _ebo;

    if(oneTime) glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    if(oneTime){
        auto now = std::chrono::high_resolution_clock::now();
        glGenBuffers(1, &_vbo);
        glGenBuffers(1, &_ebo);
        std::cout << "Spent " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - now).count() << " ms\n";
    }

    static std::vector<unsigned int> indexTemplate = {0, 1, 3, 1, 2, 3};
    static int n = 0;
    float x = myX;
    float y = myY;
    float width = 50;
    float height = 50;
    float textureID = -1;
    static std::vector<Vertex> _buffer;
    static std::vector<unsigned int> _indexBuffer;
    static int vertices = 4;

    float sinVal = (sin(glfwGetTime()) + 1) / 2.0f;
    float cosVal = (cos(glfwGetTime()) + 1) / 2.0f;
    Cast::Vertex v1 = {{x, y, 1.0f},                    {cosVal, 0.0f, 1.0f}, {0.0f, 1.0f, textureID}};
    Cast::Vertex v2 = {{x + width, y, 1.0f},            {0.0f, 1.0f, sinVal}, {1.0f, 1.0f, textureID}};
    Cast::Vertex v3 = {{x + width, y - height, 1.0f},   {1.0f, cosVal, 0.0f}, {1.0f, 0.0f, textureID}};
    Cast::Vertex v4 = {{x, y - height, 1.0f},           {1.0f, sinVal, 1.0f}, {0.0f, 0.0f, textureID}};

    _buffer.push_back(v1);_buffer.push_back(v2);_buffer.push_back(v3);_buffer.push_back(v4);

    for (size_t i = 0; i < indexTemplate.size(); ++i) {
        _indexBuffer.push_back(n * vertices + indexTemplate[i]);
    }

    glBindVertexArray(_vao);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, _buffer.size() * sizeof(decltype(_buffer.front())), _buffer.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer.size() * sizeof(unsigned int), _indexBuffer.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glDrawElements(GL_TRIANGLES, _indexBuffer.size(), GL_UNSIGNED_INT, 0);
    _buffer.clear();
    _indexBuffer.clear();
    // end
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    oneTime = false;

    dyRenderer->addRectangle("Dork", Core::myX + 50, Core::myY, 50, 50, -1);
    dyRenderer->draw();


    // Check and proc events, swap render buffers
    glfwSwapBuffers(_window);
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

    _window = glfwCreateWindow(1920, 1080, "Cast", NULL, NULL);
    if (!_window) {
        std::cerr << "Failed to create GLFW _window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(_window);

    std::cout << "Done creating window\n";
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);



   
    glDisable(GL_CULL_FACE);
 

}
