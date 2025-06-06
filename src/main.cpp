#include <iostream>
#include <GameWindow.h>
#include <glad/glad.h>

const char *vertexShaderSource = 
"#version 460 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
"}\0";

const char *fragmentShaderSource = 
"#version 460 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

void inputCallback(GameWindow* window, gwInputEvent event) {
    switch (event.eventType) {
        case gw_windowReizeEvent:
            glViewport(0, 0, event.windowWidth, event.windowHeight);
            break;
        default:
            break;
    }
}

float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f
};

int main() {
    gwlPrintVersion();

    GameWindow* window = gwlCreateWindow("Voxel Survival Game :v");
    gwlCreateOpenGLContext(&window);

    // There is no loader function defined for gwl, so use this instead
    if (!gladLoadGL()) {
        std::cout << "Failed to initialize OpenGL functions\n";
    } 

    gwlSetInputCallback(window, inputCallback);
    gwlShowWindow(window);

    glClearColor(0.0f, 0.0f, 0.05f, 1.0f);

    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "SHADER COMPILATION FAILED :" << infoLog << "\n";
    }

    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "SHADER COMPILATION FAILED :" << infoLog << "\n";
    }

    GLuint shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "SHADER LINKAGE FAILED :" << infoLog << "\n";
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    while (gwlGetWindowStatus(window) == GW_WINDOW_ACTIVE) {

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        gwlSwapBuffers(window);
        gwlPollEvents(window);
    }

    gwlCleanupWindow(window);
    return 0;   
}