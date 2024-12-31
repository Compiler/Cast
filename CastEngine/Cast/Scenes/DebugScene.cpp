#include "DebugScene.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtc/type_ptr.hpp"


float DebugScene::myX = 0;
float DebugScene::myY = 0;

void DebugScene::generateEntity(float x, float y, float id, std::string name){
    auto entity = ecs_registry.create();
    auto& transform = ecs_registry.emplace<Cast::Transform>(entity);
    auto& texture = ecs_registry.emplace<Cast::Texture>(entity);
    texture.position = {0,0};
    texture.dimensions = {1,1};
    texture.id = id;
    auto& renderable = ecs_registry.emplace<Cast::Renderable>(entity);
    auto& label = ecs_registry.emplace<Cast::Named>(entity);
    ecs_registry.emplace<Cast::Collidable>(entity);
    label.entityName = name;
    renderable.color = {1, 1, 1, 1};
    transform.position.x = x;
    transform.position.y = y;
    transform.dimensions.x = 50;
    transform.dimensions.y = 50;
}

bool DebugScene::init() {

    CAST_DEBUG("WHat is {} " , 3);
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
    myX = 900;
    myY = 350;
    this->generateEntity(900, 350, 3, "Player");
    this->generateEntity(500, 350, 0, "Player2");
    this->generateEntity(150, 350, 1, "Player4");
    this->generateEntity(400, 350, 2, "Player3");
    
    for(auto&& [entity, trans, rend] : ecs_registry.view<Cast::Transform, Cast::Renderable>().each()){
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

    return true;

}

void DebugScene::update(float delta){
    // Reset collision flags
    auto view = ecs_registry.view<Cast::Collidable>();
    static int count = 0;
    if(count-- <= 0){
        for (auto entity : view) {
            std::cout << "Entity " << static_cast<uint32_t>(entity) << ": " << view.get<Cast::Collidable>(entity).isColliding << " -> " << std::bitset<8>(view.get<Cast::Collidable>(entity).bitmask) << "\n";
        }
        count = 144;
    }
    collisionSystem(ecs_registry);
    myY -= 0.998 / delta;
    if(myY < 300 + 50) myY = 300 + 50;
    for(auto&& [entity, trans, rend, named, coll] : ecs_registry.view<Cast::Transform, Cast::Renderable, Cast::Named, Cast::Collidable>().each()){
        if(named.entityName == std::string("Player")){
            if(!coll.isColliding){
                trans.position.y = myY; 
                trans.position.x = myX; 
            }else{
                auto bitset = std::bitset<8>(coll.bitmask);
                if(bitset[COLLISION_BITS::LEFT]){
                    trans.position.x = std::max(trans.position.x, myX);
                }else if(bitset[COLLISION_BITS::RIGHT]){
                    trans.position.x = std::min(trans.position.x, myX);
                }else trans.position.x = myX;
                if(bitset[COLLISION_BITS::TOP]){
                    trans.position.y = std::max(trans.position.y, myY);
                }else if(bitset[COLLISION_BITS::BOTTOM]){
                    trans.position.y = std::min(trans.position.y, myY);
                }else trans.position.y = myY;
                //std::cout << "Entity " << named.entityName << " colliding: " << coll.bitmask << std::endl;
            }
        }
    }
    dyRenderer->update();
}


void DebugScene::render(float delta) {

    glUseProgram(myShader->getUID());
    auto proj = glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f, -1.0f, 5.0f);
    glUniformMatrix4fv(glGetUniformLocation(myShader->getUID(), "u_projection"),1, GL_FALSE, glm::value_ptr(proj));


    glUniform1f(glGetUniformLocation(myShader->getUID(), "u_time"), glfwGetTime());

    renderer->draw();

    dyRenderer->render();

}

bool DebugScene::cleanup() {

    return true;
}
