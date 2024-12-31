#include "DebugScene.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtc/type_ptr.hpp"

bool DebugScene::init() {

    CAST_DEBUG("WHat is {} " , 3);
    CHECK_GL_ERROR();
    myShader = new Shader();
    CHECK_GL_ERROR();
    
    myShader->addShader(GL_VERTEX_SHADER, "Resources/Shaders/passthrough.vert");
    myShader->addShader(GL_FRAGMENT_SHADER, "Resources/Shaders/passthrough.frag");
    myShader->compile();


    CAST_LOG("{}", "Returning from DebugScene::init");
    CHECK_GL_ERROR();
    return true;

}

void DebugScene::update(float delta){

}


void DebugScene::render(float delta) {

}

bool DebugScene::cleanup() {

    return true;
}
