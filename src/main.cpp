#include <iostream>
#include <Classes/Shader.hpp>
#include <Classes/Camera.hpp>
#include <gml.hpp>
#include <GameWindow.h>
#include <glad/glad.h>
#include <Classes/Geometry/Cube.hpp>

// Icky, add as a gwl function to retrieve these values
float windowWidth = 800.0f;
float windowHeight = 600.0f;

EngineClasses::Camera sceneCamera = EngineClasses::Camera(
    gml::Vec3(0.0f, 0.0f, 3.0f),
    2.5f,
    0.05f,
    400.0f,
    300.0f
);

void inputCallback(GameWindow* window, gwInputEvent event) {
    switch (event.eventType) {
        case gw_windowReizeEvent:
            glViewport(0, 0, event.windowWidth, event.windowHeight);
            windowWidth = (float)event.windowWidth;
            windowHeight = (float)event.windowHeight;

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

    glClearColor(0.0f, 0.0f, 0.05f, 1.0f);

    EngineClasses::Cube::cubeDataInit();

    EngineClasses::Shader BasicShader("Shaders/vertexShader.vert", "Shaders/fragmentShader.frag");

    gml::Mat4 myPerspectiveMat = 
        gml::Mat4::perspectiveProjection(.78539816f, windowWidth/windowHeight, 0.1f, 50.0f);

    GLuint modelMatLocation = 
        glGetUniformLocation(BasicShader.getID(), "modelMat");

    GLuint viewMatLocation = 
        glGetUniformLocation(BasicShader.getID(), "viewMat");

    GLuint perspectiveMatLocation = 
        glGetUniformLocation(BasicShader.getID(), "perspectiveMat");

    double lastTime = gwlGetTime(window);
    double elapsedTime;


    EngineClasses::Cube myCube = EngineClasses::Cube(gml::Vec3(0.0f, 0.0f, 0.0f));
    EngineClasses::Cube mySecondsCube = EngineClasses::Cube(gml::Vec3(3.0f, 0.25f, -0.77f));

    while (gwlGetWindowStatus(window) == GW_WINDOW_ACTIVE) {

        glClear(GL_COLOR_BUFFER_BIT);

        myCube.bindVBO();
        myCube.bindVAO();

        BasicShader.use();
        glUniformMatrix4fv(modelMatLocation, 1, GL_FALSE, myCube.getModelMat().getData()); 

        glUniformMatrix4fv(viewMatLocation, 1, GL_FALSE, sceneCamera.getViewMat());

        myPerspectiveMat =  
            gml::Mat4::perspectiveProjection(0.78539816f, windowWidth/windowHeight, 0.1f, 50.0f);
        
        glUniformMatrix4fv(perspectiveMatLocation, 1, GL_FALSE, myPerspectiveMat.getData());

        myCube.draw();

        glUniformMatrix4fv(modelMatLocation, 1, GL_FALSE, mySecondsCube.getModelMat().getData()); 

        mySecondsCube.draw();

        gwlSwapBuffers(window);
        gwlPollEvents(window);

        elapsedTime = gwlGetTime(window) - lastTime;
        lastTime = gwlGetTime(window);

        sceneCamera.updatePosition(elapsedTime);
    }

    gwlCleanupWindow(window);
    return 0;   
}