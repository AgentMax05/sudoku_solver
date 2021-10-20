#pragma once

#include <SDL2/SDL.h>

class Clock {
public: 
    Clock(int newRefreshRate);
    float getTicks();
    float tick();
    float getDeltaTime();

private:
    float lastTickCall = 0;
    int refreshRate;
    float deltaTime = 0;
};