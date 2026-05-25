#include <iostream>
#include <SDL2/SDL.h>  // 使用 SDL2 子目录下的 SDL.h
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "Game.h"
int main(int argc, char* argv[]) {
   Game &game = Game::getInstance();
   game.init();
   game.run();
   //clean is used in ~Game()

}