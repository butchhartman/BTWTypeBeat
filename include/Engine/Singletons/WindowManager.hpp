#ifndef ENGINE_SINGLETON_WINDOWMANAGER_H_
#define ENGINE_SINGLETON_WINDOWMANAGER_H_

#include <GameWindow.h>
namespace EngineSingletons {

class WindowManager {
    // TODO: Consider updating GWL to support having multiple callback functions (idk if this is a good idea lool)
    private:
        static const char* initWindowName;
        static inputFlagBits initInputFlags;
        static PTRINPUTCBFUNC initInputCallback;
    public:
        static void setInitWindowName(const char* name);
        static void setInitInputFlags(inputFlagBits initInputFlags);
        static void setInitInputCallback(PTRINPUTCBFUNC inputCallback);
    
    private:
        GameWindow* window;

    public:
        WindowManager(); // do nothing
        ~WindowManager(); // do nothing

        void init(); // do startup

        void showWindow();
        // void createOpenGLContext();
        void pollEvents();
        void swapBuffers();
        double getTime();
        uint64_t isWindowActive();
        void shutDown(); // do cleanup
};

} // namespace EngineSingletons

#endif // ENGINE_SINGLETON_WINDOWMANAGER_H_