// SceneMain.h
#ifndef SCENE_MAIN_H
#define SCENE_MAIN_H

#include "Scene.h"
#include <list>
#include <random>
class Game; // forward declaration,声明存在这个类，防止循环引用
class SceneMain : public Scene {
public:
    SceneMain();
    ~SceneMain();

    void update(float deltatime) override;
    void render() override;
    void handleEvent(SDL_Event* event) override;
    void init() override;
    void clean() override;
    void keyBoardControl(float deltatime);
    void shootPlayer();                          // 发射子弹
    void useSkill();                             // 使用技能
    void updatePlayerProjectiles(float deltaTime); // 更新子弹
    void renderPlayerProjectiles();              // 渲染子弹

     void spawEnemy();                         // 生成敌机
    void updateEnemies(float deltaTime);      // 更新敌机
    void renderEnemies();                     // 渲染敌机

    void renderEnemyProjectiles();             // 渲染敌机子弹
    void updateEnemyProjectiles(float deltaTime); // 更新敌机子弹
    void shootEnemy(Enemy* enemy);              // 敌机发射子弹
    SDL_FPoint getDirection(Enemy* enemy);      // 计算子弹方向

    void updatePlayer(float deltaTime);   // 更新玩家状态
    void enemyExplode(Enemy* enemy);      // 敌机爆炸效果
    void playerExplode(Player * player);                 // 玩家爆炸效果

    void updateExplosions(float deltaTime);  // 更新爆炸动画
    void renderExplosions();                 // 渲染爆炸动画

    void dropItem(Enemy* enemy);
    void updateItems(float deltaTime);
    void playerGetItem(Item* item);
    void renderItems();


private:
    Game &game;
    Player player;
    ProjectilePlayer projectilePlayerTemplate;//子弹模板
    ProjectilePlayer projectilePlayerTemplate1{600,1};
    ProjectilePlayer projectilePlayerTemplate2{600,1};
    // 存储活动子弹的列表
    std::list<ProjectilePlayer*> projectilesPlayer;

    // 随机数相关成员
    std::mt19937 gen;                         // 随机数生成器
    std::uniform_real_distribution<float> dis; // 随机数分布器
    
    // 敌机相关成员
    Enemy enemyTemplate1;                      // 敌机模板
    Enemy enemyTemplate2{2400,120,3};                      // 敌机模板
    std::list<Enemy*> enemies;                // 存储活动敌机的列表

    ProjectileEnemy projectileEnemyTemplate1;    // 敌机子弹模板
    ProjectileEnemy projectileEnemyTemplate2;    // 敌机子弹模版
    ProjectileEnemy projectileEnemyTemplate3{220,2};    // 敌机子弹模版
    std::list<ProjectileEnemy*> projectilesEnemy; // 敌机子弹容器

    bool isDead{false};

    float timeStopTime = 0.0f;   // 时停剩余时间（秒）
    bool isTimeStopped = false;  // 是否处于时停状态

    Explosion explosionTemplate;            // 爆炸动画模板
    std::list<Explosion*> explosions;       // 存储活动爆炸动画的列表

    Item itemLifeTemplate;
    Item itemShieldTemplate{ItemType::Shield};
    Item itemTimeTemplate{ItemType::Time};
    std::list<Item*> items;

    SDL_Texture * shield_photo; // 玩家护盾图片
    
};

#endif // SCENE_MAIN_H