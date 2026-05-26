// SceneEnd.h
#ifndef SCENE_END_H
#define SCENE_END_H

#include "Scene.h"
#include <string>
#include "Game.h"

class SceneEnd : public Scene{
public:
    virtual void init();
    virtual void update(float deltaTime);
    virtual void render();
    virtual void clean();
    virtual void handleEvent(SDL_Event* event);
    void removeLastUTF8Char(std::string& str);

    void renderPhase1();
    void renderPhase2();

private:
     bool isTyping = true;
    std::string name = "";
    float blinkTimer = 1.0f;  // 新增：光标闪烁计时器

};

#endif // SCENE_END_H