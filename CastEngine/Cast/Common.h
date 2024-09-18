#pragma once
#include <glad/glad.h>
#include <iostream>
#define CHECK_GL_ERROR() Cast::CHECK_FOR_ERROR(__FILE__, __LINE__)
namespace Cast{

    inline void CHECK_FOR_ERROR(const char* file, int line) {
        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR) {
            std::cerr << "OpenGL error (" << file << ":" << line << "): " << err << std::endl;
        }
    }

}
