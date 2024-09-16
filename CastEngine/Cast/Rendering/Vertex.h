#include <glm/glm.hpp>

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
