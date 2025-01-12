#include "PhongLighting.h"
#include "Cameras/FPSCamera.h"
#include "Common.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <cstddef>
#include <include/stb_image/stb_image.h>

Cast::Camera PhongLightingScene::_cam{};

bool PhongLightingScene::init() {

    glfwSetCursorPosCallback(Cast::window, mouse_callback);
    glfwSetScrollCallback(Cast::window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(Cast::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    CAST_DEBUG("WHat is {} " , 3);
    CHECK_GL_ERROR();
    _shader = new Shader();
    CHECK_GL_ERROR();
    
    _shader->addShader(GL_VERTEX_SHADER, "Resources/Shaders/lighting_object.vert");
    _shader->addShader(GL_FRAGMENT_SHADER, "Resources/Shaders/lighting_object.frag");
    _shader->compile();


    CAST_LOG("{}", "Returning from PhongLightingScene::init");
    CHECK_GL_ERROR();

    objCube.setPosition({-1.0, -2.5, -15});
    std::vector<Vertex> vertices = objCube.getVertices();

    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, position)));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, color)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, texturePosition)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normal)));
    glEnableVertexAttribArray(3);
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

    // -------------------

    
    _lightingShader.addShader(GL_VERTEX_SHADER, "Resources/Shaders/lighting.vert");
    _lightingShader.addShader(GL_FRAGMENT_SHADER, "Resources/Shaders/lighting.frag");
    _lightingShader.compile();



    lightCube.setPosition({0.0, 1.0, -5.0});
    auto lvertices = lightCube.getVertices();
    glGenVertexArrays(1, &_lvao);
    glBindVertexArray(_lvao);

    unsigned int lvbo;
    glGenBuffers(1, &lvbo);
    glBindBuffer(GL_ARRAY_BUFFER, lvbo);
    glBufferData(GL_ARRAY_BUFFER, lvertices.size() * sizeof(Vertex), lvertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, position)));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, color)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, texturePosition)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normal)));
    glEnableVertexAttribArray(3);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    CHECK_GL_ERROR();

    return true;

}

void PhongLightingScene::update(float delta){

    processInput(Cast::window);

}

void PhongLightingScene::render(float delta) {

    glBindVertexArray(_vao);
    glUseProgram(_shader->getUID());
    {
        glm::mat4 model         = glm::mat4(1.0f); 
        glm::mat4 view          = glm::mat4(1.0f);
        glm::mat4 projection    = glm::mat4(1.0f);
        //model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        static float xOff = 0;
        static float yOff = 0;
        xOff += 0.05* glm::sin(glfwGetTime());
        model = glm::translate(model, objCube.getPosition());
        model = glm::rotate(model, (float)(glfwGetTime()), glm::vec3(1, 1, 1));
        view  = _cam.GetViewMatrix();
        projection = glm::perspective(glm::radians(_cam.Zoom), (float)Cast::window_width / (float)Cast::window_height, 0.1f, 100.0f);
        unsigned int modelLoc = glGetUniformLocation(_shader->getUID(), "iModel");
        unsigned int viewLoc  = glGetUniformLocation(_shader->getUID(), "iView");
        unsigned int projLoc  = glGetUniformLocation(_shader->getUID(), "iProjection");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        //GLint location = glGetUniformLocation(_shader->getUID(), "u_time");
        //glUniform1f(location, glfwGetTime());
    }
    CHECK_GL_ERROR();

    auto objLocation = glGetUniformLocation(_shader->getUID(), "u_objectColor");
    auto lightLocation = glGetUniformLocation(_shader->getUID(), "u_lightColor");
    auto lightPosLocation = glGetUniformLocation(_shader->getUID(), "u_lightPos");
    auto viewPosLocation = glGetUniformLocation(_shader->getUID(), "u_viewPos");


    glUniform3fv(objLocation, 1, glm::value_ptr(glm::vec3{1.0f, 0.5f, 0.31f}));
    glUniform3fv(lightLocation, 1, glm::value_ptr(glm::vec3{1, 1.0, 1.0}));
    glUniform3fv(lightPosLocation, 1, glm::value_ptr(lightCube.getPosition()));
    glUniform3fv(viewPosLocation, 1, glm::value_ptr(_cam.Position));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    //-----------------------

    glUseProgram(_lightingShader.getUID());

    {
        glm::mat4 model         = glm::mat4(1.0f); 
        glm::mat4 view          = glm::mat4(1.0f);
        glm::mat4 projection    = glm::mat4(1.0f);
        //model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::translate(model, lightCube.getPosition());
        model = glm::scale(model, glm::vec3(0.2f));
        auto cubePos = lightCube.getPosition();
        glm::vec3 rotationAxis = {cubePos.x, cubePos.y, cubePos.z};
        view  = _cam.GetViewMatrix();
        projection = glm::perspective(glm::radians(_cam.Zoom), (float)Cast::window_width / (float)Cast::window_height, 0.1f, 100.0f);
        unsigned int modelLoc = glGetUniformLocation(_shader->getUID(), "iModel");
        unsigned int viewLoc  = glGetUniformLocation(_shader->getUID(), "iView");
        unsigned int projLoc  = glGetUniformLocation(_shader->getUID(), "iProjection");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    }


    glBindVertexArray(_lvao);
    glDrawArrays(GL_TRIANGLES, 0, 36);

}

bool PhongLightingScene::cleanup() {

    return true;
}


void PhongLightingScene::processInput(GLFWwindow *window){

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
void PhongLightingScene::mouse_callback(GLFWwindow* window, double xposIn, double yposIn){
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
void PhongLightingScene::scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    _cam.ProcessMouseScroll(static_cast<float>(yoffset));
}

