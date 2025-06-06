#include <iostream>
#include <Classes/Shader.hpp>
#include <GameWindow.h>
#include <glad/glad.h>


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

    EngineClasses::Shader BasicShader("Shaders/vertexShader.vert", "Shaders/fragmentShader.frag");

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

        BasicShader.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        gwlSwapBuffers(window);
        gwlPollEvents(window);
    }

    gwlCleanupWindow(window);
    return 0;   
}