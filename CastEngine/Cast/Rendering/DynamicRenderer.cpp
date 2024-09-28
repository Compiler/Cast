
#include "DynamicRenderer.h"
#include "ECS/BasicComponents.h"

DynamicRenderer::DynamicRenderer(){

    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ebo);

}

void DynamicRenderer::addRectangle(const glm::vec4& position, const glm::vec4& dimensions, const glm::vec4& color, Cast::Texture& texture) {
    static std::vector<int> indexTemplate = {0, 1, 3, 1, 2, 3};
    static int n = 0;
    static int vertices = 4;

    const glm::vec2& texturePosition = texture.position;
    const glm::vec2& textureDimensions = texture.dimensions;
    const float& textureID = texture.id;

    
    Cast::Vertex v1 = {{position.x, position.y, position.z, 1.0f},                              color, {texturePosition.x, texturePosition.y + textureDimensions.y, textureID, 0.0f}}; // 0 1
    Cast::Vertex v2 = {{position.x + dimensions.x, position.y,position.z,  1.0f},               color, {texturePosition.x + textureDimensions.x, texturePosition.y + textureDimensions.y, textureID, 0.0f}}; // 1 1
    Cast::Vertex v3 = {{position.x + dimensions.x, position.y - dimensions.y,position.z,  1.0f},color, {texturePosition.x + textureDimensions.x, texturePosition.y, textureID, 0.0f}}; // 1 0
    Cast::Vertex v4 = {{position.x, position.y - dimensions.y,position.z,  1.0f},               color, {texturePosition.x, texturePosition.y, textureID, 0.0f}}; // 0 0

    _buffer.push_back(v1);_buffer.push_back(v2);_buffer.push_back(v3);_buffer.push_back(v4);

    for (size_t i = 0; i < indexTemplate.size(); ++i) {
        _indexBuffer.push_back(n * vertices + indexTemplate[i]);
    }
}

void DynamicRenderer::update() {
    for(auto&& [entity, trans, texture, rend] : Cast::ecs_registry.view<Cast::Transform, Cast::Texture, Cast::Renderable>().each()){
        addRectangle(trans.position, trans.dimensions, rend.color, texture);;
    }
}
void DynamicRenderer::render() {
    glBindVertexArray(_vao);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, _buffer.size() * sizeof(decltype(_buffer.front())), _buffer.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer.size() * sizeof(unsigned int), _indexBuffer.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);


    glDrawElements(GL_TRIANGLES, _indexBuffer.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    _buffer.clear();
    _indexBuffer.clear();
}

