#include <Engine/Singletons/EngineSystemsManager.hpp>

namespace EngineSingletons {

    void EngineSystemsManager::init() {
        this->windowManager.init();
        this->renderingManager.init();

        this->windowManager.showWindow();
    }

    void EngineSystemsManager::shutDown() {
        this->renderingManager.shutDown();
        this->windowManager.shutDown();
    }


} // namespace EngineSingletons