#ifndef ENGINE_CLASSES_DRAWABLEOBJECT_H_
#define ENGINE_CLASSES_DRAWABLEOBJECT_H_

#include <Engine/Classes/Camera.hpp>

namespace EngineClasses {

class DrawableObject {
    public:
        virtual void draw(EngineClasses::Camera camera) = 0; // pure virutal
};


} // namespace EngineClasses

#endif // ENGINE_CLASSES_DRAWABLEOBJECT_H_