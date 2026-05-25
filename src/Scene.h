// Scene.h
#ifndef SCENE_H
#define SCENE_H

#include <SDL2/SDL.h>
#include "Object.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

class Scene{
public:
    Scene() = default;
    virtual ~Scene() = default;

    virtual void init() = 0;
    virtual void update(float deltatime) = 0;
    virtual void render() = 0;
    virtual void clean() = 0;
    virtual void handleEvent(SDL_Event* event) = 0;
};

#endif // SCENE_H