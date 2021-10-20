#include "RenderWindow.hpp"

#include <iostream>

RenderWindow::RenderWindow(const char* winTitle, int winWidth, int winHeight) {
    window = NULL;
    renderer = NULL;

    window = SDL_CreateWindow(winTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, winWidth, winHeight, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        std::cout << "Window failed to init. Error: " << SDL_GetError() << "\n";
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

SDL_Texture* RenderWindow::loadTexture(const char* filePath) {
    SDL_Texture* texture = NULL;
    texture = IMG_LoadTexture(renderer, filePath);
    if (texture == NULL) {
        std::cout << "Failed to load texture. Error: " << SDL_GetError() << "\n";
    }
    return texture;
}

void RenderWindow::clear() {
    SDL_RenderClear(renderer);
}

void RenderWindow::display() {
    SDL_RenderPresent(renderer);
}

void RenderWindow::cleanUp() {
    SDL_DestroyWindow(window);
}

void RenderWindow::render(Entity& entity) {
    SDL_Rect dest;
    dest.x = entity.getX();
    dest.y = entity.getY();
    dest.w = entity.getW();
    dest.h = entity.getH();

    SDL_Rect src = entity.getCurrentFrame();

    SDL_RenderCopyEx(renderer, entity.getTexture(), &src, &dest, 0, NULL, SDL_FLIP_NONE);
}

