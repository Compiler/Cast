#include "LightingScene.h"
#include "Cameras/FPSCamera.h"
#include "Common.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <cstddef>
#include <include/stb_image/stb_image.h>

Cast::Camera LightingScene::_cam{};

bool LightingScene::init() {

    glfwSetCursorPosCallback(Cast::window, mouse_callback);
    glfwSetScrollCallback(Cast::window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(Cast::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    CAST_DEBUG("WHat is {} " , 3);
    CHECK_GL_ERROR();
    _shader = new Shader();
    CHECK_GL_ERROR();
    
    _shader->addShader(GL_VERTEX_SHADER, "Resources/Shaders/passthrough.vert");
    _shader->addShader(GL_FRAGMENT_SHADER, "Resources/Shaders/passthrough.frag");
    _shader->compile();


    CAST_LOG("{}", "Returning from LightingScene::init");
    CHECK_GL_ERROR();

    cube.setPosition({0.0, -0.5, -5});
    std::vector<Vertex> vertices = cube.getVertices();

    unsigned int indices[] = {
        0, 1, 3, 
        1, 2, 3  
    };

    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, position)));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, color)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, texturePosition)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
    // Unbind after VAO to keep the binds in the state, otherwise segfault
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    CHECK_GL_ERROR();

    unsigned int texture;
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Set texture wrapping/filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load the texture image
    int width, height, nrChannels;
    unsigned char *data = stbi_load("Resources/Assets/container.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;  // Determine format based on number of channels
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0 + 0);
        glBindTexture(GL_TEXTURE_2D, texture);
    } else {
        std::cout << "Failed to load texture: " << "Resources/assets/dirt.png" << std::endl;
        return false;
    }
    stbi_image_free(data);


    _lightingShader.addShader(GL_VERTEX_SHADER, "Resources/Shaders/blinn_phong.vert");
    _lightingShader.addShader(GL_FRAGMENT_SHADER, "Resources/Shaders/blinn_phong.frag");
    _lightingShader.compile();

    glGenVertexArrays(1, &_lvao);
    glBindVertexArray(_lvao);

    unsigned int lvbo;
    glGenBuffers(1, &lvbo);
    glBindBuffer(GL_ARRAY_BUFFER, lvbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, position)));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, color)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, texturePosition)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    CHECK_GL_ERROR();

    return true;

}

void LightingScene::update(float delta){

    processInput(Cast::window);

}

void LightingScene::render(float delta) {
    glUseProgram(_shader->getUID());

    glm::mat4 model         = glm::mat4(1.0f); 
    glm::mat4 view          = glm::mat4(1.0f);
    glm::mat4 projection    = glm::mat4(1.0f);
    //model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    auto cubePos = cube.getPosition();
    glm::vec3 rotationAxis = {cubePos.x, cubePos.y, cubePos.z};
    model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), rotationAxis);
    view  = _cam.GetViewMatrix();
    projection = glm::perspective(glm::radians(_cam.Zoom), (float)Cast::window_width / (float)Cast::window_height, 0.1f, 100.0f);
    unsigned int modelLoc = glGetUniformLocation(_shader->getUID(), "iModel");
    unsigned int viewLoc  = glGetUniformLocation(_shader->getUID(), "iView");
    unsigned int projLoc  = glGetUniformLocation(_shader->getUID(), "iProjection");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    GLint location = glGetUniformLocation(_shader->getUID(), "u_time");
    glUniform1f(location, glfwGetTime());

    glBindVertexArray(_vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);

}

bool LightingScene::cleanup() {

    return true;
}


void LightingScene::processInput(GLFWwindow *window){

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        _cam.ProcessKeyboard(Cast::FORWARD, Cast::frameTimeMs);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        _cam.ProcessKeyboard(Cast::BACKWARD, Cast::frameTimeMs);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        _cam.ProcessKeyboard(Cast::LEFT, Cast::frameTimeMs);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        _cam.ProcessKeyboard(Cast::RIGHT, Cast::frameTimeMs);
}
void LightingScene::mouse_callback(GLFWwindow* window, double xposIn, double yposIn){
    static bool firstMouse = true;
    static float lastX = 0;
    static float lastY = 0;
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    _cam.ProcessMouseMovement(xoffset, yoffset);
}
void LightingScene::scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    _cam.ProcessMouseScroll(static_cast<float>(yoffset));
}
