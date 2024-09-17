#pragma once
#include <glad/glad.h>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
namespace Cast{
    class ResourceManager{

        public:
            static inline std::string readFile(std::string filepath){
                std::ifstream file(filepath);
                if(!file){
                    std::cerr << "Failed to load file " << filepath << std::endl;
                    return "";
                }
                std::ostringstream ss;
                ss << file.rdbuf();
                return ss.str();
            }

    };

}
