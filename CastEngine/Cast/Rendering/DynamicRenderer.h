#pragma once
#include <Cast/Common.h>
#include <Cast/Cast.h>
#include "../../include/glad/glad.h"
#include <vector>
#include <Cast/ECS/BasicComponents.h>
class DynamicRenderer{

    private:
        std::vector<Cast::Vertex> _buffer;
        std::vector<unsigned int> _indexBuffer;
        std::vector<unsigned int> _textures;
        unsigned int _ebo, _vao, _vbo;

    public:

        DynamicRenderer();
        void addRectangle(const glm::vec4& position, const glm::vec4& dimensions, const glm::vec4& color, Cast::Texture& texture);
        void update();
        void render();

        void unbind();
        inline unsigned int numArrays() const { return _indexBuffer.size(); }

};
