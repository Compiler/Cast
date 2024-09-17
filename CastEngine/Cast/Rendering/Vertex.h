#pragma once
#include <glm/glm.hpp>

#include "../../include/stb_image/stb_image.h"
namespace Cast{

    struct Point{
        float x, y, z;
    };

    struct Vertex{
        Point position;
        Point color;
        Point textureCoordinates;
    };

}
