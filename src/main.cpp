#include <iostream>
#include <Classes/Shader.hpp>
#include <Classes/Camera.hpp>
#include <gml.hpp>
#include <GameWindow.h>
#include <glad/glad.h>
#include <Classes/Geometry/Cube.hpp>
#include <Classes/Filesystem/FileReader.h>

// Icky, add as a gwl function to retrieve these values
float windowWidth = 800.0f;
float windowHeight = 600.0f;

EngineClasses::Camera sceneCamera = EngineClasses::Camera(
    gml::Vec3(0.0f, 0.0f, 3.0f),
    2.5f,
    0.05f,
    400.0f,
    300.0f,
    0.78539816f,
    windowWidth/windowHeight,
    100.0f
);

void inputCallback(GameWindow* window, gwInputEvent event) {
    switch (event.eventType) {
        case gw_windowReizeEvent:
            glViewport(0, 0, event.windowWidth, event.windowHeight);
            windowWidth = (float)event.windowWidth;
            windowHeight = (float)event.windowHeight;
            sceneCamera.setAspectRatio(windowWidth/windowHeight);
            sceneCamera.setMouseLockPosition(windowWidth/2, windowHeight/2);
            break;

        case gw_keyboardEvent:
            if (event.key == gw_W && event.keyStateFlags & KEY_DOWN_BIT)  {
                sceneCamera.setForward(true);
            } else if (event.key == gw_W && event.keyStateFlags & KEY_UP_BIT) {
                sceneCamera.setForward(false);
            }
            if (event.key == gw_A && event.keyStateFlags & KEY_DOWN_BIT)  {
                sceneCamera.setLeft(true);
            } else if (event.key == gw_A && event.keyStateFlags & KEY_UP_BIT) {
                sceneCamera.setLeft(false);
            }
            if (event.key == gw_S && event.keyStateFlags & KEY_DOWN_BIT)  {
                sceneCamera.setBackward(true);
            } else if (event.key == gw_S && event.keyStateFlags & KEY_UP_BIT) {
                sceneCamera.setBackward(false);
            }
            if (event.key == gw_D && event.keyStateFlags & KEY_DOWN_BIT)  {
                sceneCamera.setRight(true);
            } else if (event.key == gw_D && event.keyStateFlags & KEY_UP_BIT) {
                sceneCamera.setRight(false);
            }
            break;

        case gw_mouseEvent:
            sceneCamera.updateRotation(event.xPos, event.yPos);
            break;

        default:
            break;
    }
}

const float triangleVertices[] = {
    0.0f, 0.5f, 0.0f, 0.5f, 1.0f,
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f
};

int main() {
    gwlPrintVersion();

    GameWindow* window = gwlCreateWindow("Voxel Survival Game :v");
    gwlSetInputFlags(window, CAPTURE_MOUSE_BIT, 1);
    gwlCreateOpenGLContext(&window);

    // There is no loader function defined for gwl, so use this instead
    if (!gladLoadGL()) {
        std::cout << "Failed to initialize OpenGL functions\n";
    } 

    gwlSetInputCallback(window, inputCallback);
    gwlPrintGLVersion(window);
    gwlShowWindow(window);

    glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    EngineClasses::Cube::cubeDataInit();

    EngineClasses::Shader BasicShader("Shaders/vertexShader.vert", "Shaders/fragmentShaderGreen.frag");
    EngineClasses::Shader BasicShader2("Shaders/vertexShader.vert", "Shaders/fragmentShaderBlue.frag");

    double lastTime = gwlGetTime(window);
    double elapsedTime;


    EngineClasses::Cube myCube = EngineClasses::Cube(gml::Vec3(0.0f, 0.0f, 0.0f), &BasicShader);
    EngineClasses::Cube mySecondsCube = EngineClasses::Cube(gml::Vec3(3.0f, 0.25f, -0.77f), &BasicShader2);

    uint32_t bmpWidth;
    uint32_t bmpHeight;
    unsigned char* bmpData = readBMP("a.bmp", &bmpWidth, &bmpHeight);

    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmpWidth, bmpHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, bmpData);
    glGenerateMipmap(GL_TEXTURE_2D);

    free(bmpData);

    EngineClasses::Shader texShader = EngineClasses::Shader("Shaders/texTestVertShader.vert", "Shaders/texTestFragShader.frag");

    GLuint texTriVBO;
    glGenBuffers(1, &texTriVBO);
    glBindBuffer(GL_ARRAY_BUFFER, texTriVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

    GLuint texTriVAO;
    glGenVertexArrays(1, &texTriVAO);
    glBindVertexArray(texTriVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    while (gwlGetWindowStatus(window) == GW_WINDOW_ACTIVE) {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // myCube.draw(sceneCamera);
        // mySecondsCube.draw(sceneCamera);

        texShader.use();
        glBindTexture(GL_TEXTURE_2D, textureID);
        glBindBuffer(GL_ARRAY_BUFFER, texTriVBO);
        glBindVertexArray(texTriVAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        gwlSwapBuffers(window);
        gwlPollEvents(window);

        elapsedTime = gwlGetTime(window) - lastTime;
        lastTime = gwlGetTime(window);

        sceneCamera.updatePosition(elapsedTime);
    }

    gwlCleanupWindow(window);
    return 0;   
}