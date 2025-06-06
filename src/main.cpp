#include <iostream>
#include <GameWindow.h>

int main() {
    gwlPrintVersion();

    GameWindow* window = gwlCreateWindow("Voxel Survival Game :v");
    gwlCreateOpenGLContext(&window);
    gwlShowWindow(window);

    while (gwlGetWindowStatus(window) == GW_WINDOW_ACTIVE) {

        gwlSwapBuffers(window);
        gwlPollEvents(window);
    }

    gwlCleanupWindow(window);
    return 0;   
}