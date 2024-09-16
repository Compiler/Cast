#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
namespace Cast{
    inline std::string readFile(std::string filepath){
        std::ifstream file(filepath);
        if(!file){
            std::cerr << "Failed to load file " << filepath << std::endl;
            return "";
        }
        std::ostringstream ss;
        ss << file.rdbuf();
        return ss.str();
    }
}
