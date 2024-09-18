#include "Core.h"
#include "Common.h"
#include "ECS/BasicComponents.h"
#include "Rendering/DynamicRenderer.h"
#include "Rendering/StaticRenderer.h"
#include <algorithm>

float Core::myX = 0;
float Core::myY = 0;
float Cast::frameTimeMs = 0;
int Core::init(){
   
    renderer = new StaticRenderer();
    CHECK_GL_ERROR();
    dyRenderer = new DynamicRenderer();
    CHECK_GL_ERROR();
    myShader = new Shader();
    CHECK_GL_ERROR();

    renderer->addRectangle(-1, 1, 2, 2, 3);
    renderer->addRectangle(-0.5, 0, 0.25, 0.25, 0);
    renderer->addRectangle(-0.25, 0, 0.25, 0.25, 1);
    renderer->addRectangle(0, 0, 0.25, 0.25, 2);
    renderer->addRectangle(0.25, 0, 0.25, 0.25, 3);

    float sz = 0.10;
    float startingY = -0.75;
    for(float x = -2 ; x <= 2; x += sz) {
        for(float y = startingY; y >= -1.5; y -= sz) {
            renderer->addRectangle(x, y, sz, sz, y == startingY ? 1 : 2);
        }
    }

    //ENTT

    auto entity = registry.create();

    auto transform = registry.emplace<Transform>(entity);
    registry.emplace<Renderable>(entity);
    
    transform.position.x = 0.5;
    transform.position.y = 0.5;

    for(auto&& [entity, trans, rend] : registry.view<Transform, Renderable>().each()){
        renderer->addRectangle(trans.position.x, trans.position.y, 0.5, 0.5, -1);
    }
    CHECK_GL_ERROR();
    myShader->addShader(GL_VERTEX_SHADER, "Resources/Shaders/passthrough.vert");
    CHECK_GL_ERROR();
    myShader->addShader(GL_FRAGMENT_SHADER, "Resources/Shaders/passthrough.frag");
    CHECK_GL_ERROR();
    myShader->compile();
    CHECK_GL_ERROR();

    glUseProgram(myShader->getUID());

    CHECK_GL_ERROR();
    renderer->addTexture("Resources/Assets/spritesheet.png");
    renderer->addTexture("Resources/Assets/grass_jpg.jpg");
    renderer->addTexture("Resources/Assets/dirt.png");
    renderer->addTexture("Resources/Assets/landscape_mountains.png");

    CHECK_GL_ERROR();

    glUniform1i(glGetUniformLocation(myShader->getUID(), "u_texture1"), 0);
    glUniform1i(glGetUniformLocation(myShader->getUID(), "u_texture2"), 1);
    glUniform1i(glGetUniformLocation(myShader->getUID(), "u_texture3"), 2);
    glUniform1i(glGetUniformLocation(myShader->getUID(), "u_texture4"), 3);

    CHECK_GL_ERROR();


    renderer->preDraw();
    CHECK_GL_ERROR();

    return 0;

}


void Core::update(){

    myY -= 0.01 / frameTimeMs;
    if(myY < -0.5) myY = -0.5;
}
void Core::render(){
        CHECK_GL_ERROR();
        // Pre process 
        processInput(_window);

        // Rendering commands
        glClearColor(0.7, 0.5, 0.8, 1);
        glClear(GL_COLOR_BUFFER_BIT);


        glUseProgram(myShader->getUID());
        glUniform1f(glGetUniformLocation(myShader->getUID(), "u_time"), glfwGetTime());
        renderer->draw();
        glUseProgram(myShader->getUID());
        glUniform1f(glGetUniformLocation(myShader->getUID(), "u_time"), glfwGetTime());
        //dyRenderer->addRectangle("Dork", Core::myX, 0, 0.2, 0.2, -1);
        //dyRenderer->draw();


        // static
        static bool oneTime = true;
        static unsigned int _vao, _vbo, _ebo;

        if(oneTime) glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);

if(oneTime){
        glGenBuffers(1, &_vbo);
        glGenBuffers(1, &_ebo);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
}
        static std::vector<int> indexTemplate = {0, 1, 3, 1, 2, 3};
        static int n = 0;
        float x = myX;
        float y = myY;
        float width = 0.25;
        float height = 0.25;
        float textureID = -1;
        static std::vector<Vertex> _buffer;
        static std::vector<unsigned int> _indexBuffer;
        static int vertices = 4;

        Cast::Vertex v1 = {{x, y, 1.0f},                    {1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, textureID}};
        Cast::Vertex v2 = {{x + width, y, 1.0f},            {0.0f, 1.0f, 1.0f}, {1.0f, 1.0f, textureID}};
        Cast::Vertex v3 = {{x + width, y - height, 1.0f},   {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, textureID}};
        Cast::Vertex v4 = {{x, y - height, 1.0f},           {1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, textureID}};

        _buffer.push_back(v1);_buffer.push_back(v2);_buffer.push_back(v3);_buffer.push_back(v4);

        for (size_t i = 0; i < indexTemplate.size(); ++i) {
            _indexBuffer.push_back(n * vertices + indexTemplate[i]);
        }

        glBindVertexArray(_vao);

        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, _buffer.size() * sizeof(decltype(_buffer.front())), _buffer.data(), GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer.size() * sizeof(int), _indexBuffer.data(), GL_DYNAMIC_DRAW);

        glDrawElements(GL_TRIANGLES, _indexBuffer.size(), GL_UNSIGNED_INT, 0);
        _buffer.clear();
        _indexBuffer.clear();
        // end
        glBindVertexArray(0);


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
