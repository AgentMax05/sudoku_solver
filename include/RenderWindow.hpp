#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Entity.hpp"

class RenderWindow {
public:
    RenderWindow(const char* winTitle, int winWidth, int winHeight);
    SDL_Texture* loadTexture(const char* filePath);
    void cleanUp();
    void clear();
    void display();
    void render(Entity& entity);

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
};