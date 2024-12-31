#include "DebugScene.h"
#include "Common.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtc/type_ptr.hpp"

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
        -0.5, -0.5, 0.0f,
        0.5, -0.5, 0.0f,
        0, 0.5, 0.0f
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    // Unbind after VAO to keep the binds in the state, otherwise segfault
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    CHECK_GL_ERROR();

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
