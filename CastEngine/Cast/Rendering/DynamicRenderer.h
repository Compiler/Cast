#pragma once
#include "../../include/glad/glad.h"
#include <vector>
#include <string>
#include <Cast/ECS/BasicComponents.h>
#include <iostream>
class DynamicRenderer{

    private:
        std::vector<Cast::Vertex> _buffer;
        std::vector<unsigned int> _indexBuffer;
        std::vector<unsigned int> _textures;
        unsigned int _ebo, _vao, _vbo;

        void updateBuffer(const std::vector<Cast::Vertex> newRect);

    public:

        DynamicRenderer();
        void addRectangle(std::string name, float x, float y, float width, float height, float textureID);
        void draw();
        void preDraw();
        bool addTexture(std::string filePath);
        inline unsigned int numArrays() const { return _indexBuffer.size(); }

};
