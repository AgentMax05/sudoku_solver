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

void Entity::setX(float xP) {
    x = xP;
}

void Entity::setY(float yP) {
    y = yP;
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

void Entity::setTexture(SDL_Texture* newTexture) {
    texture = newTexture;
}

SDL_Rect Entity::getCurrentFrame() {
    return currentFrame;
}

void Entity::setCurrentFrame(SDL_Rect newFrame) {
    currentFrame = newFrame;
}

int Entity::getAngle() {
    return angle;
}

void Entity::setAngle(int newAngle) {
    angle = newAngle;
}

bool Entity::isClicked(float xP, float yP) {
    return (
        xP < x + w &&
        xP > x &&
        yP < y + h &&
        yP > y
    );
}