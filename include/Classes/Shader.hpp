#ifndef ENGINE_SHADER_H_
#define ENGINE_SHADER_H_

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

#include <glad/glad.h>

namespace EngineClasses {

class Shader {
    private:
        GLuint programID;
    public:
        Shader(const std::string vertexShaderPath, const std::string fragmentShaderPath);

        ~Shader();

        void use();
        GLuint getID();
};

}

#endif // ENGINE_SHADER_H_