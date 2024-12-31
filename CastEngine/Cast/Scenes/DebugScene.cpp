#include "DebugScene.h"
#include "Common.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <include/stb_image/stb_image.h>

bool DebugScene::init() {

    CAST_DEBUG("WHat is {} " , 3);
    CHECK_GL_ERROR();
    _shader = new Shader();
    CHECK_GL_ERROR();
    
    _shader->addShader(GL_VERTEX_SHADER, "Resources/Shaders/passthrough.vert");
    _shader->addShader(GL_FRAGMENT_SHADER, "Resources/Shaders/passthrough.frag");
    _shader->compile();


    CAST_LOG("{}", "Returning from DebugScene::init");
    CHECK_GL_ERROR();

    float vertices[] = {
        -1, -1, 0.0f, 1.0f,     1.0f, 0.0f, 0.0f, 1.0f,     0, 0, 0.0f, 1.0f,
        1, -1, 0.0f, 1.0f,      1.0f, 0.0f, 0.0f, 1.0f,     1, 0, 0.0f, 1.0f,
        0, 1, 0.0f, 1.0f,         1.0f, 0.0f, 0.0f, 1.0f,   0.5, 0.5, 0.0f, 1.0f
    };
    int indices[] = {
        0, 1, 2
    };

    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(4 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(8*sizeof(float)));
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


    return true;

}

void DebugScene::update(float delta){

    

}


void DebugScene::render(float delta) {
    glUseProgram(_shader->getUID());
    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

}

bool DebugScene::cleanup() {

    return true;
}
