#pragma once
#include "glm/ext/vector_float3.hpp"
#include <glad/glad.h>
#include <Cast/Common.h>
#include <Cast/Utilities/ResourceManager.h>
#include <vector>
class Shader{

    private:
        unsigned int _uid;
        char _infoLog[512];
        std::vector<unsigned int> _shaders;

        std::string _glTypeToString(unsigned int type);


    public:
        Shader();
        
        void addShader(unsigned int type, std::string filepath);
        void compile();

        void setVec3(const char* uniformName, glm::vec3& vec);
        void setVec3(const char* uniformName, glm::vec3 vec);

        inline unsigned int getUID() const { return _uid; };

};
