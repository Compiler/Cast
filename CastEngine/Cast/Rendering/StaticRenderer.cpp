#define STB_IMAGE_IMPLEMENTATION
#include "StaticRenderer.h"
#include <iostream>
#include <include/stb_image/stb_image.h>

StaticRenderer::StaticRenderer(){
    
}

void StaticRenderer::preDraw(){
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);


    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, _buffer.size() * sizeof(decltype(_buffer.front())), _buffer.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer.size() * sizeof(int), _indexBuffer.data(), GL_STATIC_DRAW);



    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

}

void StaticRenderer::addRectangle(float x, float y, float width, float height, float textureID){
    static std::vector<int> indexTemplate = {0, 1, 3, 1, 2, 3};
    static int vertices = 4;
    //int currentOffsetMultiplier = _buffer.size() / 9;
    static int n = 0;


    Cast::Vertex v1 = {x, y, 1.0f,                    1.0f, 1.0f, 1.0f,       0.0f, 1.0f, textureID};
    Cast::Vertex v2 = {x + width, y, 1.0f,            1.0f, 1.0f, 1.0f,       1.0f, 1.0f, textureID};
    Cast::Vertex v3 = {x + width, y - height, 1.0f,   1.0f, 1.0f, 1.0f,       1.0f, 0.0f, textureID};
    Cast::Vertex v4 = {x, y - height, 1.0f,           1.0f, 1.0f, 1.0f,       0.0f, 0.0f, textureID};
    _buffer.push_back(v1);
    _buffer.push_back(v2);
    _buffer.push_back(v3);
    _buffer.push_back(v4);

    // 0, 1, 3, 1, 2, 3
    for(size_t i = 0; i < indexTemplate.size(); i++){
        _indexBuffer.push_back(n * vertices + indexTemplate[i]);
    }
    n++;
}

void StaticRenderer::draw(){

        {GLenum err;while ((err = glGetError()) != GL_NO_ERROR)std::cerr << "SR60:OpenGL error: " << err << std::endl;}
    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, _indexBuffer.size(), GL_UNSIGNED_INT, 0);
        {GLenum err;while ((err = glGetError()) != GL_NO_ERROR)std::cerr << "SR62:OpenGL error: " << err << std::endl;}
}

bool StaticRenderer::addTexture(std::string filepath) {
    unsigned int texture;
    stbi_set_flip_vertically_on_load(true);   
    glActiveTexture(GL_TEXTURE15);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Set texture wrapping/filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load the texture image
    int width, height, nrChannels;
    unsigned char *data = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;  // Determine format based on number of channels
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        _textures.push_back(texture);
        std::cout << "Bound texture " << texture << " to " << _textures.size() - 1 << std::endl;
        glActiveTexture(GL_TEXTURE0 + _textures.size() - 1);
        glBindTexture(GL_TEXTURE_2D, texture);
    } else {
        std::cout << "Failed to load texture: " << filepath << std::endl;
        return false;
    }
    stbi_image_free(data);

    return true;
}
