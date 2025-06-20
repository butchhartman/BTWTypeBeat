#ifndef ENGINE_SHADER_H_
#define ENGINE_SHADER_H_

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

#include <glad.h>

// TODO: Have the shader include, as a member, the location of its model, view, and projection matrix and remove the location members from the cube classes.
namespace EngineClasses {

class Shader {
    private:
        GLuint programID;
    public:
        Shader();
        Shader(const std::string vertexShaderPath, const std::string fragmentShaderPath);

        ~Shader();

        void use();
        GLuint getID();
};

}

#endif // ENGINE_SHADER_H_