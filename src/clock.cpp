#include "Clock.hpp"

Clock::Clock(int newRefreshRate) {
    refreshRate = newRefreshRate;
}

float Clock::getTicks() {
    return SDL_GetTicks();
}

float Clock::tick() {
    float frameTicks = getTicks() - lastTickCall;
    if (frameTicks < 1000 / refreshRate) {
        SDL_Delay(1000 / refreshRate - frameTicks);
    }
    lastTickCall = getTicks();
    deltaTime = frameTicks;
    return frameTicks;
}

float Clock::getDeltaTime() {
    return deltaTime;
}