#include <iostream>
#include <SDL2/SDL.h>  // 使用 SDL2 子目录下的 SDL.h
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
int main(int argc, char* argv[]) {
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return -1;
    }
    SDL_Window * window = SDL_CreateWindow("shooter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) != (IMG_INIT_PNG | IMG_INIT_JPG)) {
        std::cerr << "IMG_Init Error: " << IMG_GetError() << std::endl;
        return -2;
    }

    SDL_Texture * texture = IMG_LoadTexture(renderer, "../assets/image/bg.png");
    if (texture == nullptr) {
        std::cerr << "IMG_LoadTexture Error: " << IMG_GetError() << std::endl;
        std::cerr << "当前工作目录可能不正确，请确保从项目根目录运行程序" << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return -3;
    }

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Mix_OpenAudio Error: " << Mix_GetError() << std::endl;
    }
    Mix_Music * music = Mix_LoadMUS("../assets/music/03_Racing_Through_Asteroids_Loop.ogg"); //Mix_Music播放时解码，只能一个；Mix_Chunk播放时解码，可以多个
    if(music == nullptr) {
        std::cerr << "Mix_LoadMUS Error: " << Mix_GetError() << std::endl;
    }
    Mix_PlayMusic(music, -1);//-1表示无限循环

    if(TTF_Init() == -1) {
        std::cerr << "TTF_Init Error: " << TTF_GetError() << std::endl;
    }
    TTF_Font * font = TTF_OpenFont("../assets/font/VonwaonBitmap-12px.ttf", 24);//字体大小在载入时确定
    if(font == nullptr) {
        std::cerr << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
    }
    //创建字体纹理
    SDL_Color color = {128, 255, 255, 255};//字体颜色
    SDL_Surface * surface = TTF_RenderUTF8_Solid(font, "Hello SDL2", color);//字
    if (surface == nullptr) {
        std::cerr << "TTF_RenderUTF8_Solid Error: " << TTF_GetError() << std::endl;
    }
    SDL_Texture * text_texture = SDL_CreateTextureFromSurface(renderer, surface);//字体纹理
    // 记录文字的实际尺寸，然后释放 surface
    int text_width = surface->w;
    int text_height = surface->h;

    while (true) {
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            break;
        }
    }
    // 清屏 清除上一帧的缓存
    SDL_RenderClear(renderer);

    // 先画背景
    SDL_RenderCopy(renderer, texture, NULL, NULL);//第三参数：纹理位置；第四参数，画到窗口的那个位置

    // 画文字（使用文字的实际尺寸）
    SDL_Rect text_rect = {100, 100, text_width, text_height};
    SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);

    // 再画前景物体（画一个长方形）——注意不要遮挡文字
    SDL_Rect rect = {100, 300, 200, 200};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);//设置画笔颜色
    SDL_RenderFillRect(renderer, &rect);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);//调回画笔颜色来画屏幕

    // 更新屏幕
    SDL_RenderPresent(renderer); // 将缓存的内容绘制到屏幕上（同时画屏幕），之后的绘制都画到缓存中
}
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(text_texture);
    TTF_CloseFont(font);
    TTF_Quit();
    Mix_FreeMusic(music);
    Mix_CloseAudio();
    Mix_Quit();
    SDL_DestroyTexture(texture);
    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

}