#pragma once

#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_float4.hpp"
#include <vector>

struct Vertex{
    glm::vec4 position;
    glm::vec4 color;
    glm::vec4 texturePosition;
    glm::vec4 normal;
};


class Cube{
    private:
        // unit cube
    std::vector<Vertex> _vertices = {

        { {0.0f, 0.0f, 0.0f, 1.0f},  {1.f, 0.f, 0.f, 1.f},  {0.f, 0.f, 0.f, 0.f},  {0.f, 0.f, -1.f, 0.f} },
        { {1.0f, 0.0f, 0.0f, 1.0f},  {1.f, 0.f, 0.f, 1.f},  {1.f, 0.f, 0.f, 0.f},  {0.f, 0.f, -1.f, 0.f} },
        { {1.0f, 1.0f, 0.0f, 1.0f},  {1.f, 0.f, 0.f, 1.f},  {1.f, 1.f, 0.f, 0.f},  {0.f, 0.f, -1.f, 0.f} },

        { {1.0f, 1.0f, 0.0f, 1.0f},  {1.f, 0.f, 0.f, 1.f},  {1.f, 1.f, 0.f, 0.f},  {0.f, 0.f, -1.f, 0.f} },
        { {0.0f, 1.0f, 0.0f, 1.0f},  {1.f, 0.f, 0.f, 1.f},  {0.f, 1.f, 0.f, 0.f},  {0.f, 0.f, -1.f, 0.f} },
        { {0.0f, 0.0f, 0.0f, 1.0f},  {1.f, 0.f, 0.f, 1.f},  {0.f, 0.f, 0.f, 0.f},  {0.f, 0.f, -1.f, 0.f} },

        { {0.0f, 0.0f, 1.0f, 1.0f},  {1.f, 0.f, 0.f, 1.f},  {0.f, 0.f, 0.f, 0.f},  {0.f, 0.f, 1.f, 0.f} },
        { {1.0f, 0.0f, 1.0f, 1.0f},  {1.f, 0.f, 0.f, 1.f},  {1.f, 0.f, 0.f, 0.f},  {0.f, 0.f, 1.f, 0.f} },
        { {1.0f, 1.0f, 1.0f, 1.0f},  {1.f, 0.f, 0.f, 1.f},  {1.f, 1.f, 0.f, 0.f},  {0.f, 0.f, 1.f, 0.f} },

        { {1.0f, 1.0f, 1.0f, 1.0f},  {1.f, 0.f, 0.f, 1.f},  {1.f, 1.f, 0.f, 0.f},  {0.f, 0.f, 1.f, 0.f} },
        { {0.0f, 1.0f, 1.0f, 1.0f},  {1.f, 0.f, 0.f, 1.f},  {0.f, 1.f, 0.f, 0.f},  {0.f, 0.f, 1.f, 0.f} },
        { {0.0f, 0.0f, 1.0f, 1.0f},  {1.f, 0.f, 0.f, 1.f},  {0.f, 0.f, 0.f, 0.f},  {0.f, 0.f, 1.f, 0.f} },

        { {0.0f, 0.0f, 0.0f, 1.0f},  {1.f, 0.f, 0.f, 1.f},  {0.f, 0.f, 0.f, 0.f},  {-1.f, 0.f, 0.f, 0.f} },
        { {0.0f, 1.0f, 0.0f, 1.0f},  {1.f, 0.f, 0.f, 1.f},  {0.f, 1.f, 0.f, 0.f},  {-1.f, 0.f, 0.f, 0.f} },
        { {0.0f, 1.0f, 1.0f, 1.0f},  {1.f, 0.f, 0.f, 1.f},  {0.f, 1.f, 0.f, 0.f},  {-1.f, 0.f, 0.f, 0.f} },

        { {0.0f, 1.0f, 1.0f, 1.0f},  {1.f, 0.f, 0.f, 1.f},  {0.f, 1.f, 0.f, 0.f},  {-1.f, 0.f, 0.f, 0.f} },
        { {0.0f, 0.0f, 1.0f, 1.0f},  {1.f, 0.f, 0.f, 1.f},  {0.f, 0.f, 0.f, 0.f},  {-1.f, 0.f, 0.f, 0.f} },
        { {0.0f, 0.0f, 0.0f, 1.0f},  {1.f, 0.f, 0.f, 1.f},  {0.f, 0.f, 0.f, 0.f},  {-1.f, 0.f, 0.f, 0.f} },

        { {1.0f, 0.0f, 0.0f, 1.0f},  {1.f, 0.f, 0.f, 1.f},  {0.f, 0.f, 0.f, 0.f},  {1.f, 0.f, 0.f, 0.f} },
        { {1.0f, 1.0f, 0.0f, 1.0f},  {1.f, 0.f, 0.f, 1.f},  {0.f, 1.f, 0.f, 0.f},  {1.f, 0.f, 0.f, 0.f} },
        { {1.0f, 1.0f, 1.0f, 1.0f},  {1.f, 0.f, 0.f, 1.f},  {0.f, 1.f, 0.f, 0.f},  {1.f, 0.f, 0.f, 0.f} },

        { {1.0f, 1.0f, 1.0f, 1.0f},  {1.f, 0.f, 0.f, 1.f},  {0.f, 1.f, 0.f, 0.f},  {1.f, 0.f, 0.f, 0.f} },
        { {1.0f, 0.0f, 1.0f, 1.0f},  {1.f, 0.f, 0.f, 1.f},  {0.f, 0.f, 0.f, 0.f},  {1.f, 0.f, 0.f, 0.f} },
        { {1.0f, 0.0f, 0.0f, 1.0f},  {1.f, 0.f, 0.f, 1.f},  {0.f, 0.f, 0.f, 0.f},  {1.f, 0.f, 0.f, 0.f} },

        { {0.0f, 0.0f, 0.0f, 1.0f},  {1.f, 0.f, 0.f, 1.f},  {0.f, 0.f, 0.f, 0.f},  {0.f, -1.f, 0.f, 0.f} },
        { {0.0f, 0.0f, 1.0f, 1.0f},  {1.f, 0.f, 0.f, 1.f},  {0.f, 1.f, 0.f, 0.f},  {0.f, -1.f, 0.f, 0.f} },
        { {1.0f, 0.0f, 1.0f, 1.0f},  {1.f, 0.f, 0.f, 1.f},  {1.f, 1.f, 0.f, 0.f},  {0.f, -1.f, 0.f, 0.f} },

        { {1.0f, 0.0f, 1.0f, 1.0f},  {1.f, 0.f, 0.f, 1.f},  {1.f, 1.f, 0.f, 0.f},  {0.f, -1.f, 0.f, 0.f} },
        { {1.0f, 0.0f, 0.0f, 1.0f},  {1.f, 0.f, 0.f, 1.f},  {1.f, 0.f, 0.f, 0.f},  {0.f, -1.f, 0.f, 0.f} },
        { {0.0f, 0.0f, 0.0f, 1.0f},  {1.f, 0.f, 0.f, 1.f},  {0.f, 0.f, 0.f, 0.f},  {0.f, -1.f, 0.f, 0.f} },

        { {0.0f, 1.0f, 0.0f, 1.0f},  {1.f, 0.f, 0.f, 1.f},  {0.f, 0.f, 0.f, 0.f},  {0.f, 1.f, 0.f, 0.f} },
        { {0.0f, 1.0f, 1.0f, 1.0f},  {1.f, 0.f, 0.f, 1.f},  {0.f, 1.f, 0.f, 0.f},  {0.f, 1.f, 0.f, 0.f} },
        { {1.0f, 1.0f, 1.0f, 1.0f},  {1.f, 0.f, 0.f, 1.f},  {1.f, 1.f, 0.f, 0.f},  {0.f, 1.f, 0.f, 0.f} },

        { {1.0f, 1.0f, 1.0f, 1.0f},  {1.f, 0.f, 0.f, 1.f},  {1.f, 1.f, 0.f, 0.f},  {0.f, 1.f, 0.f, 0.f} },
        { {1.0f, 1.0f, 0.0f, 1.0f},  {1.f, 0.f, 0.f, 1.f},  {1.f, 0.f, 0.f, 0.f},  {0.f, 1.f, 0.f, 0.f} },
        { {0.0f, 1.0f, 0.0f, 1.0f},  {1.f, 0.f, 0.f, 1.f},  {0.f, 0.f, 0.f, 0.f},  {0.f, 1.f, 0.f, 0.f} },

    };  

        glm::vec3 _position = {0.0, 0.0, 0.0};

    public:
        void setPosition(glm::vec3 position) {
            _position = glm::vec4(position.x, position.y, position.z, 1.0);
        }

        glm::vec3 getPosition() { return _position;}
        std::vector<Vertex> getVertices() const {
            //std::vector<Vertex> newVerts = _vertices;
            //for(Vertex& vertex : newVerts) {
             //   vertex.position = vertex.position + _position;
            //}
            // handling in model
            return _vertices;

        }


};
