#include "../../include/glad/glad.h"
#include <vector>
#include <string>
#include "../../include/stb_image/stb_image.h"
#include <iostream>
#include "Vertex.h"
class StaticRenderer{

    private:
        std::vector<Cast::Vertex> _buffer;
        std::vector<unsigned int> _indexBuffer;
        std::vector<unsigned int> _textures;
        unsigned int _ebo, _vao, _vbo;


    public:

        StaticRenderer();
        void addRectangle(float x, float y, float width, float height, float textureID);
        void draw();
        void preDraw();
        bool addTexture(std::string filePath);
        inline unsigned int numArrays() const { return _indexBuffer.size(); }

};