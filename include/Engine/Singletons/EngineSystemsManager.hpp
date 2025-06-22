#ifndef ENGINE_SINGLETONS_ENGINESYSTEMMANAGER_H_
#define ENGINE_SINGLETONS_ENGINESYSTEMMANAGER_H_

#include <Engine/Singletons/WindowManager.hpp>
#include <Engine/Singletons/RenderingManager.hpp>

namespace EngineSingletons {

struct EngineSystemsManager {
        WindowManager windowManager;
        RenderingManager renderingManager;

        // calls inits of all member singletons
        void init();
        // calls shutdown of all member singletons
        void shutDown();

};

} // namespace EngineSingletons

#endif // ENGINE_SINGLETONS_ENGINESYSTEMMANAGER_H_