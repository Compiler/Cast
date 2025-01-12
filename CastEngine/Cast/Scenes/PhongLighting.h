#pragma once
#include <Cast/Cast.h>
#include <Cast/Common.h>

#include <math.h>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <Cast/Rendering/Shader.h>
#include <Cast/Rendering/StaticRenderer.h>
#include <Cast/Rendering/DynamicRenderer.h>
#include <Cast/Cameras/FPSCamera.h>
#include <box2d/box2d.h>
#include <Cast/Scenes/Scene.h>
#include <Cast/Shapes/Cube.h>

#include <entt/entt.hpp>
#include <Cast/ECS/BasicComponents.h>
#include <Cast/ECS/BasicSystems.h>

class PhongLightingScene : public Scene {

    private:
        
        static Cast::Camera _cam;
        Shader* _shader;    
        Shader _lightingShader;    
        Cube lightCube, objCube;
        unsigned int _vao, _ebo;
        unsigned int _lvao;
        void generateEntity(float x, float y, float id, std::string name);

        void processInput(GLFWwindow *window);
        static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
        static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


    public:
        bool init();
        void update(float delta);
        void render(float delta);
        bool cleanup();
        ~PhongLightingScene(){}

};
