#include <glad/glad.h>
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

        inline unsigned int getUID() const { return _uid; };

};
