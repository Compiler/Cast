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

    struct Point{
        float x, y, z;
    };

    struct Texture{
        glm::vec2 position;
        glm::vec2 dimensions;
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

    struct Triangle{
        Vertex v1, v2, v3;
    };

    struct Rectangle{
        Vertex v1, v2, v3;
    };

    
    struct Renderable{
        glm::vec4 color;
        // Enum shape
    };

}
