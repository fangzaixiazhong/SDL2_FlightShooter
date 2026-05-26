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
    //初始化字体库
    if(TTF_Init() == -1) {  
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "TTF could not initialize! SDL_Error: %s\n", SDL_GetError());
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
    SDL_RenderSetLogicalSize(renderer, windowWidth, windowHeight);
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

    nearStars.texture = IMG_LoadTexture(renderer, "../assets/image/Stars-A.png");
    SDL_QueryTexture(nearStars.texture, NULL, NULL, &nearStars.width, &nearStars.height);   
    nearStars.height /= 2;
    nearStars.width /= 2;

    farStars.texture = IMG_LoadTexture(renderer, "../assets/image/Stars-B.png");
    SDL_QueryTexture(farStars.texture, NULL, NULL, &farStars.width, &farStars.height);
    farStars.height /= 2;
    farStars.width /= 2;
    farStars.speed = 20;  // 远处的星星移动速度较慢

    titleFont = TTF_OpenFont("../assets/font/VonwaonBitmap-16px.ttf", 64);
    textFont = TTF_OpenFont("../assets/font/VonwaonBitmap-16px.ttf", 32);
    if (titleFont == nullptr || textFont == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "TTF_OpenFont: %s\n", TTF_GetError());
        isRunning = false;
    }

    currentScene = new SceneTitle();
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
    if (nearStars.texture != nullptr){
        SDL_DestroyTexture(nearStars.texture);
    }
    if (farStars.texture != nullptr){
        SDL_DestroyTexture(farStars.texture);
    }
    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::changeScene(Scene *scene)
{
    if(currentScene == scene) return;
    if (currentScene != nullptr)
    {
        currentScene->clean();
        delete currentScene;
    }
    currentScene = scene;
    currentScene->init();
}
void Game::update(float d)
{
    backgroundUpdate(d);
    currentScene->update(d);
}

void Game::render()
{
    // 清空
    SDL_RenderClear(renderer);
    // 渲染星空背景
    renderBackground();

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
        if (event->type == SDL_KEYDOWN){
            if (event->key.keysym.scancode == SDL_SCANCODE_F4){
                isFullscreen = !isFullscreen;
                if (isFullscreen){
                    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
                }else{
                    SDL_SetWindowFullscreen(window, 0);
                }
            }
        }
        currentScene->handleEvent(event);
    }
}
void Game::backgroundUpdate(float deltaTime)
{
    nearStars.offset += nearStars.speed * deltaTime;
    if (nearStars.offset >= 0)
    {
        nearStars.offset -= nearStars.height;
    }

    farStars.offset += farStars.speed * deltaTime;
    if (farStars.offset >= 0){
        farStars.offset -= farStars.height;
    }
}
// Game.cpp
void Game::renderBackground()
{
    // 渲染远处的星星
    for (int posY = static_cast<int>(farStars.offset); posY < getWindowHeight(); posY += farStars.height){
        for (int posX = 0; posX < getWindowWidth(); posX += farStars.width){
            SDL_Rect ds = {posX, posY, farStars.width, farStars.height};
            SDL_RenderCopy(renderer, farStars.texture, NULL, &ds);
        }
    }
    // 渲染近处的星星
    for (int posY = static_cast<int>(nearStars.offset); posY < getWindowHeight(); posY += nearStars.height)
    {
        for (int posX = 0; posX < getWindowWidth(); posX += nearStars.width)
        {
            SDL_Rect dstRect = {posX, posY, nearStars.width, nearStars.height};
            SDL_RenderCopy(renderer, nearStars.texture, nullptr, &dstRect);
        }   
    }
}

// Game.cpp
SDL_Point Game::renderTextCentered(std::string text, float posY, bool isTitle)
{
    SDL_Color color = {255, 255, 255, 255};
    SDL_Surface *surface;
    if (isTitle){
        surface = TTF_RenderUTF8_Solid(titleFont, text.c_str(), color);
    }else{
        surface = TTF_RenderUTF8_Solid(textFont, text.c_str(), color);
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    int y = static_cast<int>((getWindowHeight() - surface->h) * posY);
    SDL_Rect rect = {getWindowWidth() / 2 - surface->w / 2,
                     y,
                     surface->w,
                     surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    return {rect.x + rect.w, y};  // 返回文本末尾的坐标
}

void Game::renderTextPos(std::string text, int posX, int posY, bool isLeft)
{
    SDL_Color color = {255, 255, 255, 255};
    SDL_Surface *surface = TTF_RenderUTF8_Solid(textFont, text.c_str(), color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect;
    if (isLeft){
        rect = {posX, posY, surface->w, surface->h};
    }else{
        rect = {getWindowWidth() - posX - surface->w, posY, surface->w, surface->h};
    }
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

// Game.cpp
void Game::insertLeaderBoard(int score, std::string name)
{
    leaderBoard.insert({score, name});
    if (leaderBoard.size() > 8){
        leaderBoard.erase(--leaderBoard.end());
    }
}