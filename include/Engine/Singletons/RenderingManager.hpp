#ifndef ENGINE_SINGLETONS_RENDERINGMANAGER_H_
#define ENGINE_SINGLETONS_RENDERINGMANAGER_H_

#include <Engine/Classes/Camera.hpp>
#include <Engine/Classes/Geometry/DrawableObject.hpp>

namespace EngineSingletons {

// TODO: Consider making this class more customizable (static members for init flags and gl settings and such)
class RenderingManager {
    private:
        double currentTime;
        double lastTime;
        EngineClasses::Camera camera;
    public:
        RenderingManager();
        ~RenderingManager();

        EngineClasses::Camera* getCameraPtr();

        void init();
        void shutDown();

        // Takes in a pointer to a class which inherits from the abstract class drawable object and calls its virtual draw function which draws that object to the screen
        void drawObject(EngineClasses::DrawableObject* objectToDraw);

        // Code that is run before anything else each frame. Eventually the engine user should be able to write code to be executed here
        void frameBegin();

        // Code that is ran last during each frame. Eventually the user should be able to write code to be executed here
        void frameEnd(double time);
};


} // namespace EngineSingletons


#endif // ENGINE_SINGLETONS_RENDERINGMANAGER_H_
