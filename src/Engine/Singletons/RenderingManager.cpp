#include <glad.h>
#include <stdexcept>
#include <Engine/Classes/Camera.hpp>
#include <Engine/Classes/Geometry/Cube.hpp>
#include <Engine/Classes/Geometry/TexturedCube.hpp>
#include <Engine/Singletons/RenderingManager.hpp>

namespace EngineSingletons {

    RenderingManager::RenderingManager() : camera(gml::Vec3(0.0f, 0.0f, 0.0f), 0, 0, 0, 0, 0, 0, 0) {

    }

    RenderingManager::~RenderingManager() {

    }

    EngineClasses::Camera* RenderingManager::getCameraPtr() {
        return &(this->camera);
    }

    void RenderingManager::init() {
        this->camera = EngineClasses::Camera(
            gml::Vec3(0.0f, 0.0f, 0.0f),
            2.5f,
            0.05f,
            0.0f,
            0.0f,
            1.78539816f,
            16.0f/9.0f,
            100.0f
        );

        // There is no loader function defined for gwl, so use this instead
        if (!gladLoadGL()) {
            std::cout << "Failed to initialize OpenGL functions\n";
            throw std::runtime_error("OpenGL functions loading failed");
        } 

        glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
        glEnable(GL_DEPTH_TEST);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);

        EngineClasses::Cube::cubeDataInit();
        EngineClasses::TexturedCube::texturedCubeDataInit();
        
    }

    void RenderingManager::shutDown() {

    }

    void RenderingManager::drawObject(EngineClasses::DrawableObject* objectToDraw) {
        objectToDraw->draw(this->camera);
    }

    void RenderingManager::frameBegin() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void RenderingManager::frameEnd(double time) {
        this->currentTime = time;
        double elapsedTime = currentTime - lastTime;
        this->lastTime = currentTime;

        this->camera.updatePosition(elapsedTime);
    }

} // namespace EngineSingletons
