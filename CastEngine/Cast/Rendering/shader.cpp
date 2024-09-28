#include "Shader.h"

Shader::Shader(){
}
void Shader::addShader(unsigned int type, std::string filepath){
    int success;
    unsigned int shader = glCreateShader(type);

    std::string shaderSourceCode = Cast::ResourceManager::readFile(filepath);
    const char* sourceCodeCStr = shaderSourceCode.c_str();
    glShaderSource(shader, 1, &sourceCodeCStr, NULL);
    glCompileShader(shader);

    // Check for success
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, _infoLog);
        std::cerr << "ShaderError::" << _glTypeToString(type) << "::CompilationFailed\t" << _infoLog << std::endl;
    } else {
        std::cout << "Shader::" << _glTypeToString(type) << " compiled successfully\n";
        _shaders.push_back(shader);
    }
}


void Shader::compile() {
    _uid = glCreateProgram();

    for (auto shader : _shaders) glAttachShader(_uid, shader);

    glLinkProgram(_uid);
    CHECK_GL_ERROR();

    for (auto shader : _shaders) glDeleteShader(shader);

    int success;
    char infoLogShader[512];
    glGetProgramiv(_uid, GL_LINK_STATUS, &success);  
    if (!success) {
        glGetProgramInfoLog(_uid, 512, NULL, infoLogShader);
        std::cerr << "ShaderError::ShaderProgram::LinkFailed\t" << infoLogShader << std::endl;
    } else {
        std::cout << "Shader::Program compiled successfully\n";
    }

    CHECK_GL_ERROR();
}



std::string Shader::_glTypeToString(unsigned int type){
    if(type == GL_VERTEX_SHADER) return "Vertex";
    if(type == GL_FRAGMENT_SHADER) return "Fragment";

    return "UndefinedType";
}
