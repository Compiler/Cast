#pragma once
#include <Cast/Cast.h>
#include <Cast/Common.h>

#include <math.h>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <Cast/Rendering/Shader.h>
#include <Cast/Rendering/StaticRenderer.h>
#include <Cast/Rendering/DynamicRenderer.h>
#include <Cast/Cameras/PerspectiveCamera.h>
#include <box2d/box2d.h>
#include <Cast/Scenes/Scene.h>

#include <entt/entt.hpp>
#include <Cast/ECS/BasicComponents.h>
#include <Cast/ECS/BasicSystems.h>

class DebugScene : public Scene {

    private:
        
        Cast::Camera _cam;
        Shader* _shader;    
        unsigned int _vao, _ebo;
        void generateEntity(float x, float y, float id, std::string name);

    public:
        bool init();
        void update(float delta);
        void render(float delta);
        bool cleanup();
        ~DebugScene(){}

};
