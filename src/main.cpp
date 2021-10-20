#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <vector>

#include "RenderWindow.hpp"
#include "Clock.hpp"

using namespace std;

const int SCREENWIDTH = 800, SCREENHEIGHT = 800;
const int REFRESHRATE = 60;

int mainloop(RenderWindow& window);

int main(int argc, char* args[]) {

    // initialize sdl video
    if (SDL_Init(SDL_INIT_VIDEO) > 0) {
        cout << "SDL INIT FAILED. SDL_ERROR: " << SDL_GetError() << "\n";
    }
    if (!IMG_Init(IMG_INIT_PNG)) {
        cout << "SDL IMAGE INIT FAILED. SDL_ERROR: " << SDL_GetError() << "\n";
    }
    
    RenderWindow window("Game Window", SCREENWIDTH, SCREENHEIGHT);

    mainloop(window);

    window.cleanUp();
    IMG_Quit();
    SDL_Quit();

    return 0;
}

int mainloop(RenderWindow& window) {
    bool mainloopRunning = true;
    SDL_Event event;
    Clock clock(REFRESHRATE);

    while (mainloopRunning) {
        clock.tick();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return 0;
            }
        }

        window.clear();
        window.display();
    }
}