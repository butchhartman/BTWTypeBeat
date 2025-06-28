#include <iostream>
#include <Engine/Classes/Shader.hpp>
#include <Engine/Classes/Camera.hpp>
#include <Engine/Classes/Texture.hpp>
#include <gml.hpp>
#include <GameWindow.h>
#include <glad.h>
#include <Engine/Classes/Geometry/Cube.hpp>
#include <Engine/Classes/Geometry/TexturedCube.hpp>
#include <Engine/Filesystem/FileReader.h>
#include <Engine/Singletons/WindowManager.hpp>
#include <Engine/Singletons/EngineSystemsManager.hpp>

#include <Game/Classes/Chunk.hpp>
#include <Game/Classes/ChunkManager.hpp>


EngineSingletons::EngineSystemsManager sysMan;

void inputCallback(GameWindow* window, gwInputEvent event) {
    switch (event.eventType) {
        case gw_windowReizeEvent:
            glViewport(0, 0, event.windowWidth, event.windowHeight);
            sysMan.renderingManager.getCameraPtr()->setAspectRatio((float)event.windowWidth/(float)event.windowHeight);
            sysMan.renderingManager.getCameraPtr()->setMouseLockPosition((float)event.windowWidth/2.0f, (float)event.windowHeight/2.0f);
            break;

        case gw_keyboardEvent:
            if (event.key == gw_W && event.keyStateFlags & KEY_DOWN_BIT)  {
                sysMan.renderingManager.getCameraPtr()->setForward(true);
            } else if (event.key == gw_W && event.keyStateFlags & KEY_UP_BIT) {
                sysMan.renderingManager.getCameraPtr()->setForward(false);
            }
            if (event.key == gw_A && event.keyStateFlags & KEY_DOWN_BIT)  {
                sysMan.renderingManager.getCameraPtr()->setLeft(true);
            } else if (event.key == gw_A && event.keyStateFlags & KEY_UP_BIT) {
                sysMan.renderingManager.getCameraPtr()->setLeft(false);
            }
            if (event.key == gw_S && event.keyStateFlags & KEY_DOWN_BIT)  {
                sysMan.renderingManager.getCameraPtr()->setBackward(true);
            } else if (event.key == gw_S && event.keyStateFlags & KEY_UP_BIT) {
                sysMan.renderingManager.getCameraPtr()->setBackward(false);
            }
            if (event.key == gw_D && event.keyStateFlags & KEY_DOWN_BIT)  {
                sysMan.renderingManager.getCameraPtr()->setRight(true);
            } else if (event.key == gw_D && event.keyStateFlags & KEY_UP_BIT) {
                sysMan.renderingManager.getCameraPtr()->setRight(false);
            }
            break;

        case gw_mouseEvent:
            sysMan.renderingManager.getCameraPtr()->updateRotation(event.xPos, event.yPos);
            break;

        default:
            break;
    }
}


int main() {
    EngineSingletons::WindowManager::setInitWindowName("Voxel Survival Game - Now With Singletons!");
    EngineSingletons::WindowManager::setInitInputFlags(CAPTURE_MOUSE_BIT);
    EngineSingletons::WindowManager::setInitInputCallback(inputCallback);

    sysMan.init();

    // TODO: Add shader management to RenderingManager singleton (Maybe create its own singleton, idk if rendering manager is ideal for this)
    EngineClasses::Shader BasicShader("Shaders/vertexShader.vert", "Shaders/fragmentShaderGreen.frag");
    // EngineClasses::Shader BasicShader2("Shaders/vertexShader.vert", "Shaders/fragmentShaderBlue.frag");

    EngineClasses::Shader texShader = EngineClasses::Shader("Shaders/texTestVertShader.vert", "Shaders/texTestFragShader.frag");

    EngineClasses::Cube myCube = EngineClasses::Cube(gml::Vec3(0.0f, 0.0f, 0.0f), &BasicShader);
    // EngineClasses::Cube mySecondsCube = EngineClasses::Cube(gml::Vec3(3.0f, 0.25f, -0.77f), &BasicShader2);

    // TODO: Add texture management to some singleton (idk if rendering manager is ideal for this or not)
    EngineClasses::Texture dirt = EngineClasses::Texture(
        "a.bmp", 
        EngineClasses::TextureFlags::TEXTURE_FILE_BMP_BIT
    );


    // EngineClasses::TexturedCube dirtCube = EngineClasses::TexturedCube(gml::Vec3(-2.0f, 0.0f, 0.5f), &texShader, &dirt);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    GameClasses::Chunk::initChunkStaticData();
    // GameClasses::Chunk testChunk = GameClasses::Chunk(gml::Vec3(0.0f, 0.0f, 0.0f) );
    // GameClasses::Chunk testChunk2 = GameClasses::Chunk(gml::Vec3(16.0f, 0.0f, 0.0f) );
    // testChunk.load();
    // testChunk2.load();
    GameClasses::ChunkManager myManager = GameClasses::ChunkManager(2);
    while (sysMan.windowManager.isWindowActive()) {

        sysMan.renderingManager.frameBegin();

        // testChunk.render(*(sysMan.renderingManager.getCameraPtr()));
        // testChunk2.render(*(sysMan.renderingManager.getCameraPtr()));
        myManager.update((sysMan.renderingManager.getCameraPtr())->getCameraPosition());
        myManager.renderLoadedChunks(*(sysMan.renderingManager.getCameraPtr()));

        sysMan.windowManager.swapBuffers();
        sysMan.windowManager.pollEvents();


        sysMan.renderingManager.frameEnd(sysMan.windowManager.getTime());
    }

    // testChunk.unload();
    // testChunk2.unload();

    sysMan.shutDown();
    return 0;   
}