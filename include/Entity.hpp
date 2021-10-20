#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Entity {
public:
    Entity(float xP, float yP, int destW, int destH, int srcW, int srcH, SDL_Texture* textureP, int newAngle = 0);

    float getX();
    float getY();
    int getW();
    int getH();

    void setX(float xP);
    void setY(float yP);

    SDL_Texture* getTexture();
    SDL_Rect getCurrentFrame();

    void setAngle(int newAngle);
    int getAngle();

private:
    float x, y, w, h;
    SDL_Texture* texture;
    SDL_Rect currentFrame;
    int angle;
};