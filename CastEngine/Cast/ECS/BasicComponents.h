#pragma once
#include <string>
#include <glm/vec4.hpp>
#include <vector>
namespace Cast{
    
    struct Transform{
        glm::vec4 position;
        glm::vec4 rotation;
    };

    struct Polygon{
        std::vector<glm::vec4> vertices;
    };

    struct Point{
        float x, y, z;
    };

    struct Vertex{
        Point position;
        Point color;
        Point textureCoordinates;
    };

    struct Triangle{
        Vertex v1, v2, v3;
    };


    struct Texture{
        float s, t, id;
        std::string filepath;
    };
    
    struct Renderable{
        glm::vec4 color;
    };

}
