#pragma once
#include <string>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <vector>
namespace Cast{
    
    struct Transform{
        glm::vec4 position;
        glm::vec4 dimensions;
        glm::vec4 rotation;
    };

    struct Named{
        std::string entityName;
    };

    struct Point{
        float x, y, z;
    };

    struct Texture{
        glm::vec2 position = {0, 0};
        glm::vec2 dimensions = {1, 1};
        float id;
        std::string filepath;
    };

    struct Vertex{
        glm::vec4 position;
        glm::vec4 color;
        glm::vec4 textureData; //s, t, id
    };

    struct Polygon{
        std::vector<Vertex> vertices;
    };

    struct Renderable{
        glm::vec4 color;
        // Enum shape
    };

}
