#include "../../include/glad/glad.h"
#include <vector>
#include <string>
#include "../../include/stb_image/stb_image.h"
#include <iostream>
#include "Vertex.h"
class StaticRenderer{

    private:
        std::vector<Cast::Vertex> _buffer;
        std::vector<unsigned int> _textures;
        unsigned int _vbo, _vao, vbo;


    public:

        bool addTexture(std::string filePath);

};
