#ifndef OBJECT_H
#define OBJECT_H
#include "SDL2/SDL.h"
struct Player{
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0, 0};
    int width = 0;
    int height = 0;
    int speed = 200;//每秒运动速度
    Uint32 coolDown = 500;        // 射击冷却时间（毫秒）
    Uint32 lastShootTime = 0;     // 上次射击的时间
    int currentHealth = 6;      // 玩家初始生命值为3
    int maxHealth{10};
    float shieldTime = 0.0f;     // 护盾剩余时间（秒）
    bool hasShield = false;      // 是否存在护盾
};
struct ProjectilePlayer{
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0, 0};
    int width = 0;
    int height = 0;
    int speed = 400;              // 子弹移动速度
        int damage = 1;             // 玩家子弹伤害值为1
};

struct Enemy{
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0, 0};
    int width = 0;
    int height = 0;
    int speed = 200;         // 敌机移动速度
    Uint32 coolDown = 1300;     // 发射冷却时间（1秒）
    Uint32 lastShootTime = 0;   // 上次发射时间
    int currentHealth = 2;      // 敌机初始生命值为2
};

struct ProjectileEnemy{
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0, 0};
    SDL_FPoint direction = {0, 0};  // 子弹方向向量
    int width = 0;
    int height = 0;
    int speed = 400;
        int damage = 1;             // 子弹伤害值为1
};

// Object.h
struct Explosion{ //可认为是多张图片拼成的，每张图片代表一帧
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0, 0};
    int width = 0;
    int height = 0;
    int currentFrame = 0;       // 当前帧索引
    int totlaFrame = 0;         // 总帧数
    Uint32 startTime = 0;       // 动画开始时间
    Uint32 FPS = 10;            // 帧率（每秒播放帧数）
};

// 物品类型枚举
enum class ItemType{
    Life,
    Shield,
    Time,
    
};

// 物品结构体
struct Item{
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0, 0};
    SDL_FPoint direction = {0, 0};
    int width = 0;
    int height = 0;
    int speed = 200;
    int bounceCount = 3;
    ItemType type = ItemType::Life;
    Item(ItemType t):type(t){}
    Item() = default;
};
#endif // OBJECT_H