#include <iostream>
#include <Classes/Shader.hpp>
#include <gml.hpp>
#include <GameWindow.h>
#include <glad/glad.h>

// Icky, add as a gwl function to retrieve these values
float windowWidth = 800.0f;
float windowHeight = 600.0f;

void inputCallback(GameWindow* window, gwInputEvent event) {
    switch (event.eventType) {
        case gw_windowReizeEvent:
            glViewport(0, 0, event.windowWidth, event.windowHeight);
            windowWidth = (float)event.windowWidth;
            windowHeight = (float)event.windowHeight;
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

    gml::Mat4 myTranslationMat = gml::Mat4::translation(0.0f, 0.0f, 0.0f);
    gml::Mat4 myViewMat = gml::Mat4::lookAt(
        gml::Vec3(4.0f, 0.0f, 3.0f),
        gml::Vec3(0.0f, 0.0f, 0.0f),
        gml::Vec3(0.0f, 1.0f, 0.0f)
    );
    // gml::Mat4 myViewMat = gml::Mat4();
    gml::Mat4 myPerspectiveMat = 
        gml::Mat4::perspectiveProjection(.78539816f, windowWidth/windowHeight, 0.1f, 400.0f);


    gml::Vec4  NDCvec 
        = myPerspectiveMat * myViewMat * myTranslationMat * gml::Vec4(-0.5f, -0.5f, 0.0f, 1.0f);

    while (gwlGetWindowStatus(window) == GW_WINDOW_ACTIVE) {

        glClear(GL_COLOR_BUFFER_BIT);

        BasicShader.use();

        GLuint modelMatLocation = 
            glGetUniformLocation(BasicShader.getID(), "modelMat");
        glUniformMatrix4fv(modelMatLocation, 1, GL_FALSE, myTranslationMat.getData());

        GLuint viewMatLocation = 
            glGetUniformLocation(BasicShader.getID(), "viewMat");
        glUniformMatrix4fv(viewMatLocation, 1, GL_FALSE, myViewMat.getData());

        myPerspectiveMat =  
            gml::Mat4::perspectiveProjection(0.78539816f, windowWidth/windowHeight, 0.1f, 400.0f);
        
        GLuint perspectiveMatLocation = 
            glGetUniformLocation(BasicShader.getID(), "perspectiveMat");
        glUniformMatrix4fv(perspectiveMatLocation, 1, GL_FALSE, myPerspectiveMat.getData());

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        gwlSwapBuffers(window);
        gwlPollEvents(window);
    }

    gwlCleanupWindow(window);
    return 0;   
}