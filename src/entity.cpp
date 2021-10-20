#include "Entity.hpp"

Entity::Entity(float xP, float yP, int destW, int destH, int srcW, int srcH, SDL_Texture* textureP, int newAngle) {
    x = xP;
    y = yP;
    w = destW;
    h = destH;
    currentFrame = {0, 0, srcW, srcH};

    texture = textureP;
    angle = newAngle;
}

float Entity::getX() {
    return x;
}

float Entity::getY() {
    return y;
}

int Entity::getW() {
    return w;
}

int Entity::getH() {
    return h;
}

SDL_Texture* Entity::getTexture() {
    return texture;
}

SDL_Rect Entity::getCurrentFrame() {
    return currentFrame;
}

int Entity::getAngle() {
    return angle;
}

void Entity::setAngle(int newAngle) {
    angle = newAngle;
}
