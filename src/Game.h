// Game.h
#ifndef GAME_H
#define GAME_H

#include "Scene.h"
#include "SDL2/SDL.h"
#include "SceneMain.h"
#include "SceneTitle.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_ttf.h"
#include "Object.h"
class Game
{
public:
    static Game& getInstance(){ //单例模式，返回唯一的实例
        static Game instance;
        return instance;
    }
    ~Game();
    void run();
    void init();
    void clean();
    void changeScene(Scene* scene);
    // 渲染文字函数
        SDL_Point renderTextCentered(std::string text, float posY, bool isTitle);
    void renderTextPos(std::string text, int posX, int posY,bool isLeft = true);
    
     void handleEvent(SDL_Event *event);
    void update(float deltatime);
    void render();
    SDL_Window * getWindow() { return window; }
    SDL_Renderer* getRenderer() { return renderer; }
    int getWindowWidth() { return windowWidth; }
    int getWindowHeight() { return windowHeight; }
    void changeAutoshoot(){
        aotoShoot = !aotoShoot;
    }
    bool getAutoShoot(){
        return aotoShoot;
    }
    void backgroundUpdate(float deltaTime);
    void renderBackground();
    // setters
    void setFinalScore(int score) { finalScore = score; }

    // getters
    int getFinalScore() { return finalScore; }

    void insertLeaderBoard(int score, std::string name); // 插入排行榜
    std::multimap<int, std::string, std::greater<int>>& getLeaderBoard() { return leaderBoard; } // 返回leaderBoard的引用
private:
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    Game();
    bool isRunning{true};
    Scene* currentScene{nullptr};
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    int windowWidth = 540;
    int windowHeight = 720;
    int fps = 60;
    float frametime;
    float deltatime{0.016};//每帧时间（秒）
    bool aotoShoot{false};
    Background nearStars;  // 近处的星星
    Background farStars;   // 远处的星星

    TTF_Font* titleFont;
    TTF_Font* textFont;
    int finalScore;

    std::multimap<int, std::string, std::greater<int>> leaderBoard;
    bool isFullscreen = false;
};

#endif