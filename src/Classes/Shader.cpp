#include <Classes/Shader.hpp>

using namespace EngineClasses;

// Essentially copied from learnopengl, but file handling is somewhat of a weakpoint for me so this taught me correct ways to do it
Shader::Shader(const std::string vertexShaderPath, const std::string fragmentShaderPath) {
    std::string vertexSource;
    std::string fragmentSource;
    std::ifstream vertexShaderSourceFile;
    std::ifstream fragmentShaderSourceFile;

    vertexShaderSourceFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentShaderSourceFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        vertexShaderSourceFile.open(vertexShaderPath);
        fragmentShaderSourceFile.open(fragmentShaderPath);

        std::stringstream vertexShaderStream, fragmentShaderStream;
        
        vertexShaderStream << vertexShaderSourceFile.rdbuf();
        fragmentShaderStream << fragmentShaderSourceFile.rdbuf();

        vertexShaderSourceFile.close();
        fragmentShaderSourceFile.close();

        vertexSource = vertexShaderStream.str();
        fragmentSource = fragmentShaderStream.str();
    } catch (std::ifstream::failure e) {
        std::cout << "SHADER FILE READ FAILED\n"; 
    }

    const char* vertexShaderSourceCString = vertexSource.c_str();
    const char* fragmentShaderSourceCString = fragmentSource.c_str();

    int success;
    char infoLog[512];

    GLuint vertexShader;
    GLuint fragmentShader;

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSourceCString, NULL);
    glShaderSource(fragmentShader, 1, &fragmentShaderSourceCString, NULL);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "SHADER COMPILATION FAILED: " << infoLog << "\n";
    }
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "SHADER COMPILATION FAILED: " << infoLog << "\n";
    }

    this->programID = glCreateProgram();
    glAttachShader(this->programID, vertexShader);
    glAttachShader(this->programID, fragmentShader);
    glLinkProgram(this->programID);

    glGetProgramiv(this->programID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(this->programID, 512, NULL, infoLog);
        std::cout << "SHADER LINKAGE FAILED" << infoLog << "\n";
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() {
    // idk
    glDeleteProgram(this->programID);
}

void Shader::use() {
    glUseProgram(this->programID);
}