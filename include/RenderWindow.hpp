#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
using std::vector;

#include "Entity.hpp"

class RenderWindow {
public:
    RenderWindow(const char* winTitle, int winWidth, int winHeight);
    SDL_Texture* loadTexture(const char* filePath);
    void cleanUp();
    void clear(vector<int> color = {0, 0, 0});
    void display();
    void render(Entity& entity);

    void drawRect(SDL_Rect rect, vector<int> color, int alpha = 255, bool fill = false);
    void drawLine(vector<vector<int>> point2, vector<int> color, int alpha = 255);

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
};