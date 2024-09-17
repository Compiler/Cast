
#include "DynamicRenderer.h"

DynamicRenderer::DynamicRenderer(){}
void DynamicRenderer::updateBuffer(const std::vector<Cast::Vertex> newRect) {
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    Cast::Vertex* ptr = (Cast::Vertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    
    if (ptr) {
        std::copy(newRect.begin(), newRect.end(), ptr);
        glUnmapBuffer(GL_ARRAY_BUFFER);
    } else {
        std::cerr << "Error: Could not map buffer!" << std::endl;
    }
}

void DynamicRenderer::addRectangle(std::string name, float x, float y, float width, float height, float textureID) {
    static std::vector<int> indexTemplate = {0, 1, 3, 1, 2, 3};
    static int vertices = 4;
    static int n = 0;

    Cast::Vertex v1 = {x, y, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, textureID};
    Cast::Vertex v2 = {x + width, y, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, textureID};
    Cast::Vertex v3 = {x + width, y - height, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, textureID};
    Cast::Vertex v4 = {x, y - height, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, textureID};

    updateBuffer({v1, v2, v3, v4});

    for (size_t i = 0; i < indexTemplate.size(); ++i) {
        //_indexBuffer.push_back(n * vertices + indexTemplate[i]);
    }
    n++;
}

void DynamicRenderer::draw() {
    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

