#include <GameWindow.h>
#include <Singletons/WindowManager.hpp>

namespace EngineSingletons {

    const char* WindowManager::initWindowName = "NULL";
    inputFlagBits WindowManager::initInputFlags = (inputFlagBits)0;
    PTRINPUTCBFUNC WindowManager::initInputCallback = NULL;

    void WindowManager::setInitWindowName(const char* name) {
        WindowManager::initWindowName = name;
    }

    void WindowManager::setInitInputFlags(inputFlagBits initInputFlags) {
        WindowManager::initInputFlags = initInputFlags;
    } 

    void WindowManager::setInitInputCallback(PTRINPUTCBFUNC inputCallback) {
        WindowManager::initInputCallback = inputCallback;
    }


    WindowManager::WindowManager() {
        // purposefully nothing
    }

    WindowManager::~WindowManager() {
        // purposefully nothing
    }

    void WindowManager::init() {
        gwlPrintVersion();
        this->window = gwlCreateWindow(WindowManager::initWindowName);
        gwlSetInputFlags(this->window, WindowManager::initInputFlags, 1);
        gwlSetInputCallback(this->window, WindowManager::initInputCallback);
        gwlCreateOpenGLContext(&(this->window));
        gwlPrintGLVersion(this->window);
    }
    
    void WindowManager::showWindow() {
        gwlShowWindow(this->window);
    }

    // void WindowManager::createOpenGLContext() {
    //     gwlCreateOpenGLContext(&(this->window));
    //     gwlPrintGLVersion(this->window);
    // }

    // TODO : Consider condensing pollEvents and swapBuffers into a single update() method 
    void WindowManager::pollEvents() {
        gwlPollEvents(this->window);
    }

    void WindowManager::swapBuffers() {
        gwlSwapBuffers(this->window);
    }

    double WindowManager::getTime() {
        return gwlGetTime(this->window);
    }

    uint64_t WindowManager::isWindowActive() {
        return gwlGetWindowStatus(this->window);
    }

    void WindowManager::shutDown() {
        gwlCleanupWindow(this->window);
    }

} // namespace EngineSingletons