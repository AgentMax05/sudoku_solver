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

void RenderWindow::clear(vector<int> color) {
    SDL_SetRenderDrawColor(renderer, color[0], color[1], color[2], 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
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

void RenderWindow::drawRect(SDL_Rect rect, vector<int> color, int alpha, bool fill) {
    SDL_SetRenderDrawColor(renderer, color[0], color[1], color[2], alpha);
    if (fill) {
        SDL_RenderFillRect(renderer, &rect);
    } else {
        SDL_RenderDrawRect(renderer, &rect);
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

void RenderWindow::drawLine(vector<vector<int>> points, vector<int> color, int alpha) {
    SDL_SetRenderDrawColor(renderer, color[0], color[1], color[2], alpha);
    SDL_RenderDrawLine(renderer, points[0][0], points[0][1], points[1][0], points[1][1]);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

void RenderWindow::drawOpaqueRect(SDL_Rect rect, vector<int> color, bool fill) {
    SDL_BlendMode blendMode;
    SDL_GetRenderDrawBlendMode(renderer, &blendMode);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, color[0], color[1], color[2], color[3]);
    if (fill) {
        SDL_RenderFillRect(renderer, &rect);
    } else {
        SDL_RenderDrawRect(renderer, &rect);
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_SetRenderDrawBlendMode(renderer, blendMode);
}