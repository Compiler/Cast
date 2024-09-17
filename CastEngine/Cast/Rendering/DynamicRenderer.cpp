
#include "DynamicRenderer.h"

DynamicRenderer::DynamicRenderer(){

        // Generate and bind the VBO
    glGenBuffers(1, &_vbo);
    // Allocate space for the buffer (16 vertices)
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Cast::Vertex) * 4, NULL, GL_DYNAMIC_DRAW);

    // Generate and bind the EBO
    glGenBuffers(1, &_ebo);

    // Set up VAO and attribute pointers (assuming _vao is generated elsewhere)
    glBindVertexArray(_vao);

    // Bind VBO again before setting vertex attribute pointers
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    // Set up attribute pointers (adjust sizes/types as per Cast::Vertex definition)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Cast::Vertex), (void*)offsetof(Cast::Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Cast::Vertex), (void*)offsetof(Cast::Vertex, color));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Cast::Vertex), (void*)offsetof(Cast::Vertex, textureCoordinates));
    glEnableVertexAttribArray(2);

    // Bind EBO to VAO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    static std::vector<int> indexTemplate = {0, 1, 3, 1, 2, 3};
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexTemplate.size() * sizeof(unsigned int), indexTemplate.data(), GL_DYNAMIC_DRAW);
}

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

