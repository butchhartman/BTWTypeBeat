#include <iostream>
#include <Classes/Shader.hpp>
#include <gml.hpp>
#include <GameWindow.h>
#include <glad/glad.h>
#define RAD(angleMeasure) angleMeasure * (3.14159265358979323846 / 180.0)


// Icky, add as a gwl function to retrieve these values
float windowWidth = 800.0f;
float windowHeight = 600.0f;

const float cameraSpeed = 2.5f;
const float sensitivity = 0.05f;

float yaw = -90.0f;
float pitch = 0.0f;

float xOffset = 0.0f;
float yOffset = 0.0f;
float lastX = 0.0f;
float lastY = 0.0f;

gml::Vec3 cameraPos = gml::Vec3(0.0f, 0.0f, 3.0f);
gml::Vec3 cameraFront = gml::Vec3(0.0f, 0.0f, -1.0f);
gml::Vec3 cameraUp = gml::Vec3(0.0f, 1.0f, 0.0f);

bool forward = false;
bool backward = false;
bool right = false;
bool left = false;

float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f
};

void inputCallback(GameWindow* window, gwInputEvent event) {
    switch (event.eventType) {
        case gw_windowReizeEvent:
            glViewport(0, 0, event.windowWidth, event.windowHeight);
            windowWidth = (float)event.windowWidth;
            windowHeight = (float)event.windowHeight;
            lastX = windowWidth/2;
            lastY = windowHeight/2;
            break;

        case gw_keyboardEvent:
            if (event.key == gw_W && event.keyStateFlags & KEY_DOWN_BIT)  {
                forward = true;
            } else if (event.key == gw_W && event.keyStateFlags & KEY_UP_BIT) {
                forward = false;
            }
            if (event.key == gw_A && event.keyStateFlags & KEY_DOWN_BIT)  {
                left = true;
            } else if (event.key == gw_A && event.keyStateFlags & KEY_UP_BIT) {
                left = false;
            }
            if (event.key == gw_S && event.keyStateFlags & KEY_DOWN_BIT)  {
                backward = true;
            } else if (event.key == gw_S && event.keyStateFlags & KEY_UP_BIT) {
                backward = false;
            }
            if (event.key == gw_D && event.keyStateFlags & KEY_DOWN_BIT)  {
                right = true;
            } else if (event.key == gw_D && event.keyStateFlags & KEY_UP_BIT) {
                right = false;
            }
            break;

        case gw_mouseEvent:
            xOffset = event.xPos - lastX; 
            yOffset = lastY - event.yPos;
           
            // lastX = (float)event.xPos;
            // lastY = (float)event.yPos;

            xOffset *= sensitivity;
            yOffset *= sensitivity;
            
            // Must constrain yaw vaules to or else floating point precision
            // will be lost with high yaw values. Thanks to Lonami's comment on
            // https://learnopengl.com/Getting-started/Camera
            yaw = std::fmod(yaw + xOffset, 360.0f);
            pitch += yOffset;
            break;

        default:
            break;
    }
}

void updateCameraPos(double deltaTime) {
    if (forward) {
        cameraPos = cameraPos + cameraFront * cameraSpeed * deltaTime;
    }
    if (left) {
        cameraPos = cameraPos - (cameraFront.cross(cameraUp)).normalize() * cameraSpeed * deltaTime;
    }
    if (backward) {
        cameraPos = cameraPos - cameraFront * cameraSpeed * deltaTime;
    }
    if (right) {
        cameraPos = cameraPos + (cameraFront.cross(cameraUp)).normalize() * cameraSpeed * deltaTime;
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
        cameraPos,
        cameraPos + cameraFront,
        cameraUp
    );

    gml::Mat4 myPerspectiveMat = 
        gml::Mat4::perspectiveProjection(.78539816f, windowWidth/windowHeight, 0.1f, 400.0f);

    GLuint modelMatLocation = 
        glGetUniformLocation(BasicShader.getID(), "modelMat");

    GLuint viewMatLocation = 
        glGetUniformLocation(BasicShader.getID(), "viewMat");

    GLuint perspectiveMatLocation = 
        glGetUniformLocation(BasicShader.getID(), "perspectiveMat");

    double lastTime = gwlGetTime(window);
    double elapsedTime;

    while (gwlGetWindowStatus(window) == GW_WINDOW_ACTIVE) {
        cameraFront = gml::Vec3(
            std::cosf(RAD(yaw)) * std::cosf(RAD(pitch)),
            std::sinf(RAD(pitch)),
            std::sinf(RAD(yaw)) * std::cosf(RAD(pitch))
        ).normalize();

        glClear(GL_COLOR_BUFFER_BIT);

        BasicShader.use();

        glUniformMatrix4fv(modelMatLocation, 1, GL_FALSE, myTranslationMat.getData());

        gml::Mat4 myViewMat = gml::Mat4::lookAt(
            cameraPos,
            cameraPos + cameraFront,
            cameraUp
        );

        glUniformMatrix4fv(viewMatLocation, 1, GL_FALSE, myViewMat.getData());

        myPerspectiveMat =  
            gml::Mat4::perspectiveProjection(0.78539816f, windowWidth/windowHeight, 0.1f, 400.0f);
        
        glUniformMatrix4fv(perspectiveMatLocation, 1, GL_FALSE, myPerspectiveMat.getData());

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        gwlSwapBuffers(window);
        gwlPollEvents(window);

        elapsedTime = gwlGetTime(window) - lastTime;
        lastTime = gwlGetTime(window);

        updateCameraPos(elapsedTime);
    }

    gwlCleanupWindow(window);
    return 0;   
}