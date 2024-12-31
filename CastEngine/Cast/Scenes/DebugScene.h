#pragma once
#include <Cast/Cast.h>
#include <Cast/Common.h>

#include <math.h>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <Cast/Rendering/Shader.h>
#include <Cast/Rendering/StaticRenderer.h>
#include <Cast/Rendering/DynamicRenderer.h>
#include <box2d/box2d.h>
#include <Cast/Scenes/Scene.h>

#include <entt/entt.hpp>
#include <Cast/ECS/BasicComponents.h>
#include <Cast/ECS/BasicSystems.h>

#include <bitset>

class DebugScene : public Scene {
    private:
        entt::registry ecs_registry;
        static float myX;
        static float myY;
        static inline void glfwErrorCallback(int error, const char* description) {
            std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
        }
        StaticRenderer* renderer;
        DynamicRenderer* dyRenderer;
        Shader* myShader;    
        void generateEntity(float x, float y, float id, std::string name);

    public:
        bool init();
        void update(float delta);
        void render(float delta);
        bool cleanup();
        ~DebugScene(){}

};
