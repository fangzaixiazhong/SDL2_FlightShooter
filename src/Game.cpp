// Game.cpp
#include "Game.h"

Game::Game()
{
}

Game::~Game()
{
    clean();
}

void Game::run()
{
    while (isRunning)
    {
        auto startTime = SDL_GetTicks();
        SDL_Event event;

        handleEvent(&event);
        update(deltatime);
        render();
        auto endTime = SDL_GetTicks();
        auto elapsedTime = endTime - startTime;
        if (elapsedTime < frametime) { // 如果帧时间小于设定时间，则等待剩余时间
            SDL_Delay(frametime - elapsedTime);
            deltatime = frametime/1000.0;
        }
        else{
            deltatime = elapsedTime/1000.0;
        }
    }
    
}

void Game::init()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        isRunning = false;
    }
    if(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) != (IMG_INIT_PNG | IMG_INIT_JPG)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "IMG could not initialize! SDL_Error: %s\n", SDL_GetError());
        isRunning = false;
    }
    // 创建窗口
    window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        isRunning = false;
    }
    // 创建渲染器
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        isRunning = false;
    }
        //初始化音频
    if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) != (MIX_INIT_MP3 | MIX_INIT_OGG)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        isRunning = false;
    }

    // 打开音频设备
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_mixer could not open audio! SDL_mixer Error: %s\n", Mix_GetError());
        isRunning = false;
    }
    // 设置音效channel数量
    Mix_AllocateChannels(32);

    // 设置音乐音量
    Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
    Mix_Volume(-1, MIX_MAX_VOLUME / 8);

    currentScene = new SceneMain();
    currentScene->init();
    frametime = 1000 / fps;


}

void Game::clean()
{
    // 清理SDL_mixer
    Mix_CloseAudio();
    Mix_Quit();

    if (currentScene != nullptr)
    {
        currentScene->clean();
        delete currentScene;
    }
    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::changeScene(Scene *scene)
{
    if (currentScene != nullptr)
    {
        currentScene->clean();
        delete currentScene;
    }
    if(currentScene == scene) return;
    currentScene = scene;
    currentScene->init();
}
void Game::update(float d)
{
    currentScene->update(d);
}

void Game::render()
{
    // 清空
    SDL_RenderClear(renderer);

    currentScene->render();
    // 显示更新
    SDL_RenderPresent(renderer);
}
void Game::handleEvent(SDL_Event *event)
{
    while (SDL_PollEvent(event))
    {
        if (event->type == SDL_QUIT)
        {
            isRunning = false;
        }
        currentScene->handleEvent(event);
    }
}