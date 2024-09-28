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

    renderer->addRectangle(100, 500 + 400, 100, 100, (count++) % 4);
    renderer->addRectangle(100, 500 + 300, 100, 100, (count++) % 4);
    renderer->addRectangle(100, 500 + 200, 100, 100, (count++) % 4);
    renderer->addRectangle(100, 500 + 100, 100, 100, (count++) % 4);
    //ENTT

    auto entity = ecs_registry.create();

    auto& transform = ecs_registry.emplace<Transform>(entity);
    auto& texture = ecs_registry.emplace<Texture>(entity);
    texture.position = {0,0};
    texture.dimensions = {1,1};
    texture.id = 1;
    auto& renderable = ecs_registry.emplace<Renderable>(entity);
    auto& label = ecs_registry.emplace<Named>(entity);
    label.entityName = "Player";
    renderable.color.r = 0.54;
    transform.position.x = 0;
    transform.position.y = 350;
    transform.dimensions.x = 50;
    transform.dimensions.y = 50;

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
    for(auto&& [entity, trans, rend, named] : ecs_registry.view<Transform, Renderable, Named>().each()){
        if(named.entityName == std::string("Player")){
           trans.position.y = myY; 
           trans.position.x = myX; 
       }
    }
    dyRenderer->update();
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

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    dyRenderer->render();
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



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
