#include "Core.h"

int Core::init(){

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

    //ENTT

    auto entity = registry.create();

    auto transform = registry.emplace<Transform>(entity);
    registry.emplace<Renderable>(entity);
    
    transform.position.x = 0.5;
    transform.position.y = 0.5;

    for(auto&& [entity, trans, rend] : registry.view<Transform, Renderable>().each()){
        renderer.addRectangle(trans.position.x, trans.position.y, 0.5, 0.5, -1);
    }
    myShader.addShader(GL_VERTEX_SHADER, "Resources/Shaders/passthrough.vert");
    myShader.addShader(GL_FRAGMENT_SHADER, "Resources/Shaders/passthrough.frag");
    myShader.compile();


    glUseProgram(myShader.getUID());

    renderer.addTexture("Resources/Assets/spritesheet.png");
    renderer.addTexture("Resources/Assets/grass_jpg.jpg");
    renderer.addTexture("Resources/Assets/dirt.png");
    renderer.addTexture("Resources/Assets/landscape_mountains.png");


    glUniform1i(glGetUniformLocation(myShader.getUID(), "u_texture1"), 0);
    glUniform1i(glGetUniformLocation(myShader.getUID(), "u_texture2"), 1);
    glUniform1i(glGetUniformLocation(myShader.getUID(), "u_texture3"), 2);
    glUniform1i(glGetUniformLocation(myShader.getUID(), "u_texture4"), 3);


    renderer.preDraw();


    return 0;

}


void Core::update(){}
void Core::render(){
        // Pre process 
        processInput(_window);

        // Rendering commands
        glClearColor(0.7, 0.5, 0.8, 1);
        glClear(GL_COLOR_BUFFER_BIT);


        glUseProgram(myShader.getUID());
        glUniform1f(glGetUniformLocation(myShader.getUID(), "u_time"), glfwGetTime());
        renderer.draw();
        //rend.draw();


        // Check and proc events, swap render buffers
        glfwSwapBuffers(_window);
        glfwPollEvents();
}
