// SceneMain.cpp
#include "SceneMain.h"
#include "Game.h"
SceneMain::SceneMain():game(Game::getInstance())
{
}

SceneMain::~SceneMain()
{
}

void SceneMain::update(float deltatime)
{
    updatePlayer(deltatime);  // 添加更新玩家状态
    keyBoardControl(deltatime);
    updatePlayerProjectiles(deltatime);
    spawEnemy();                 // 生成敌机
    updateEnemies(deltatime);    // 更新敌机
    updateEnemyProjectiles(deltatime); // 更新敌机子弹
    updateExplosions(deltatime); // 更新爆炸效果
    updateItems(deltatime);  // 添加更新物品
}

void SceneMain::render()
{
        renderPlayerProjectiles();
        renderEnemyProjectiles();
    if(!isDead){
        SDL_Rect playerRect = {static_cast<int>(player.position.x), static_cast<int>(player.position.y), player.width, player.height};
    SDL_RenderCopy(game.getRenderer(), player.texture, NULL, &playerRect);
    }
    if(player.hasShield){
        SDL_Rect shieldRect = {static_cast<int>(player.position.x - 0.25 * player.width), static_cast<int>(player.position.y - 0.8 * player.height), static_cast<int>(1.5*player.width),static_cast<int>(1.5* player.height)};
        SDL_RenderCopy(game.getRenderer(), shield_photo, NULL, &shieldRect);
    }
    renderEnemies();
    renderExplosions();
    renderItems();
    
}

void SceneMain::handleEvent(SDL_Event *event)
{
    if (event->type == SDL_KEYDOWN && event->key.keysym.scancode == SDL_SCANCODE_K) {
        Game::getInstance().changeAutoshoot();
        if (Game::getInstance().getAutoShoot()) {
            // 开启自动射击时重置lastShootTime，确保立即开始射击
            player.lastShootTime = 0;
        }
    }
    if (event->type == SDL_KEYDOWN && event->key.keysym.scancode == SDL_SCANCODE_L) {
        if (player.hasSkill && !player.underSkill && !isDead) {
            useSkill();
        }
    }
}

void SceneMain::init()
{
    // 初始化随机数生成器
    std::random_device rd;  // 获取真随机数作为种子
    gen = std::mt19937(rd());  // 用种子初始化梅森旋转引擎
    dis = std::uniform_real_distribution<float>(0.0f, 1.0f);  // 设置[0,1]均匀分布

    //玩家
    player.texture = IMG_LoadTexture(game.getRenderer(), "../assets/image/SpaceShip.png");
    if (player.texture == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load SpaceShip.png! SDL_Error: %s\n", SDL_GetError());
        return;
    }
    SDL_QueryTexture(player.texture, NULL, NULL, &player.width, &player.height);//将宽高保存在传入的两个参数中
    player.width /=4;
    player.height /=4;
    player.position.x = game.getWindowWidth() / 2 - player.width / 2;
    player.position.y = game.getWindowHeight() - player.height;

    //子弹
    projectilePlayerTemplate.texture = IMG_LoadTexture(game.getRenderer(), "../assets/image/laser-1.png");
    if (projectilePlayerTemplate.texture == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load laser-1.png! SDL_Error: %s\n", SDL_GetError());
    }
    SDL_QueryTexture(projectilePlayerTemplate.texture, NULL, NULL, &projectilePlayerTemplate.width, &projectilePlayerTemplate.height);
    projectilePlayerTemplate.width /= 4;
    projectilePlayerTemplate.height /= 4;

    projectilePlayerTemplate2.texture = IMG_LoadTexture(game.getRenderer(), "../assets/image/laser-2.png");
    if (projectilePlayerTemplate2.texture == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load laser-2.png! SDL_Error: %s\n", SDL_GetError());
    }
    SDL_QueryTexture(projectilePlayerTemplate2.texture, NULL, NULL, &projectilePlayerTemplate2.width, &projectilePlayerTemplate2.height);
    projectilePlayerTemplate2.width /= 4;
    projectilePlayerTemplate2.height /= 4;
    projectilePlayerTemplate1.texture = IMG_LoadTexture(game.getRenderer(), "../assets/image/laser-3.png");
    if (projectilePlayerTemplate1.texture == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load laser-3.png! SDL_Error: %s\n", SDL_GetError());
    }
    SDL_QueryTexture(projectilePlayerTemplate1.texture, NULL, NULL, &projectilePlayerTemplate1.width, &projectilePlayerTemplate1.height);
    projectilePlayerTemplate1.width /= 4;
    projectilePlayerTemplate1.height /= 4;
    //敌机
    enemyTemplate1.texture = IMG_LoadTexture(game.getRenderer(), "../assets/image/insect-2.png");
    if (enemyTemplate1.texture == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load insect-2.png! SDL_Error: %s\n", SDL_GetError());
    }
    SDL_QueryTexture(enemyTemplate1.texture, NULL, NULL, &enemyTemplate1.width, &enemyTemplate1.height);
    enemyTemplate1.width /= 4;
    enemyTemplate1.height /= 4;

    enemyTemplate2.texture = IMG_LoadTexture(game.getRenderer(), "../assets/image/insect-1.png");
    if (enemyTemplate2.texture == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load insect-1.png! SDL_Error: %s\n", SDL_GetError());
    }
    SDL_QueryTexture(enemyTemplate2.texture, NULL, NULL, &enemyTemplate2.width, &enemyTemplate2.height);
    enemyTemplate2.width /= 3;
    enemyTemplate2.height /= 3;

    //敌机子弹
     projectileEnemyTemplate1.texture = IMG_LoadTexture(game.getRenderer(), "../assets/image/bullet-1.png");
    if (projectileEnemyTemplate1.texture == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load bullet-1.png! SDL_Error: %s\n", SDL_GetError());
    }
    SDL_QueryTexture(projectileEnemyTemplate1.texture, NULL, NULL, &projectileEnemyTemplate1.width, &projectileEnemyTemplate1.height);
    projectileEnemyTemplate1.width /= 4;
    projectileEnemyTemplate1.height /= 4;
    projectileEnemyTemplate2.texture = IMG_LoadTexture(game.getRenderer(), "../assets/image/bullet-2.png");
    if (projectileEnemyTemplate2.texture == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load bullet-2.png! SDL_Error: %s\n", SDL_GetError());
    }
    SDL_QueryTexture(projectileEnemyTemplate2.texture, NULL, NULL, &projectileEnemyTemplate2.width, &projectileEnemyTemplate2.height);
    projectileEnemyTemplate2.width /= 4;
    projectileEnemyTemplate2.height /= 4;
    projectileEnemyTemplate3.texture = IMG_LoadTexture(game.getRenderer(), "../assets/image/bullet.png");
    if (projectileEnemyTemplate3.texture == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load bullet.png! SDL_Error: %s\n", SDL_GetError());
    }
    SDL_QueryTexture(projectileEnemyTemplate3.texture, NULL, NULL, &projectileEnemyTemplate3.width, &projectileEnemyTemplate3.height);
    projectileEnemyTemplate3.width /= 3;
    projectileEnemyTemplate3.height /= 3;
    //爆炸
    explosionTemplate.texture = IMG_LoadTexture(game.getRenderer(), "../assets/effect/explosion.png");
    if (explosionTemplate.texture == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load explosion.png! SDL_Error: %s\n", SDL_GetError());
    }
    SDL_QueryTexture(explosionTemplate.texture, NULL, NULL, &explosionTemplate.width, &explosionTemplate.height);
    explosionTemplate.totlaFrame = explosionTemplate.width / explosionTemplate.height;
    explosionTemplate.width = explosionTemplate.height;

    //掉落物
    itemLifeTemplate.texture = IMG_LoadTexture(game.getRenderer(), "../assets/image/bonus_life.png");
    if (itemLifeTemplate.texture == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load bonus_life.png! SDL_Error: %s\n", SDL_GetError());
    }
    SDL_QueryTexture(itemLifeTemplate.texture, NULL, NULL, &itemLifeTemplate.width, &itemLifeTemplate.height);
    itemLifeTemplate.width /= 4;
    itemLifeTemplate.height /= 4;
    itemShieldTemplate.texture = IMG_LoadTexture(game.getRenderer(), "../assets/image/bonus_shield.png");
    if (itemShieldTemplate.texture == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load bonus_shield.png! SDL_Error: %s\n", SDL_GetError());
    }
    SDL_QueryTexture(itemShieldTemplate.texture, NULL, NULL, &itemShieldTemplate.width, &itemShieldTemplate.height);
    itemShieldTemplate.width /= 4;
    itemShieldTemplate.height /= 4;
    itemTimeTemplate.texture = IMG_LoadTexture(game.getRenderer(), "../assets/image/bonus_time.png");
    if (itemTimeTemplate.texture == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load bonus_time.png! SDL_Error: %s\n", SDL_GetError());
    }
    SDL_QueryTexture(itemTimeTemplate.texture, NULL, NULL, &itemTimeTemplate.width, &itemTimeTemplate.height);
    itemTimeTemplate.width /= 4;
    itemTimeTemplate.height /= 4;

    shield_photo = IMG_LoadTexture(game.getRenderer(), "../assets/image/shield.png");
    if (shield_photo == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load shield.png! SDL_Error: %s\n", SDL_GetError());
    }
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "SceneMain init");
}

void SceneMain::clean()
{
    for (auto &projectile : projectilesPlayer){
        if (projectile != nullptr){
            delete projectile;
        }
    }
    projectilesPlayer.clear();

    // 清理敌机
    for (auto &enemy : enemies){
        if (enemy != nullptr){
            delete enemy;
        }
    }
    enemies.clear();

    // 清理敌机子弹
    for (auto &projectile : projectilesEnemy){
        if (projectile != nullptr){
            delete projectile;
        }
    }
    projectilesEnemy.clear();

    for (auto &explosion : explosions){
        if (explosion != nullptr){
            delete explosion;
        }
    }
    explosions.clear();

    for (auto &item : items){
        if (item != nullptr){
            delete item;
        }
    }
    items.clear();

    if(player.texture != NULL){
        SDL_DestroyTexture(player.texture);
    }
    if (projectilePlayerTemplate.texture != nullptr)
    {
        SDL_DestroyTexture(projectilePlayerTemplate.texture);
    }
    if (enemyTemplate1.texture != nullptr)
    {
        SDL_DestroyTexture(enemyTemplate1.texture);
    }
    if (projectileEnemyTemplate1.texture != nullptr) {
        SDL_DestroyTexture(projectileEnemyTemplate1.texture);
    }
     if (explosionTemplate.texture != nullptr){
        SDL_DestroyTexture(explosionTemplate.texture);
    }
    if (itemLifeTemplate.texture != nullptr){
        SDL_DestroyTexture(itemLifeTemplate.texture);
    }
    if (itemShieldTemplate.texture != nullptr){
        SDL_DestroyTexture(itemShieldTemplate.texture);
    }
    if (itemTimeTemplate.texture != nullptr){
        SDL_DestroyTexture(itemTimeTemplate.texture);
    }
    if (shield_photo != nullptr){
        SDL_DestroyTexture(shield_photo);
    }
}
void SceneMain::keyBoardControl(float deltatime){
    auto keyborad = SDL_GetKeyboardState(NULL);
    if(keyborad[SDL_SCANCODE_W]){
        player.position.y -= player.speed * deltatime;
    }
    if(keyborad[SDL_SCANCODE_S]){
        player.position.y +=  player.speed * deltatime;
    }
    if(keyborad[SDL_SCANCODE_A]){
        player.position.x -=  player.speed * deltatime;
    }
    if(keyborad[SDL_SCANCODE_D]){
        player.position.x +=  player.speed * deltatime;
    }
    if(player.position.x < 0) player.position.x = 0;
if(player.position.y < 0) {
    player.position.y = 0;
}
if(player.position.x + player.width > game.getWindowWidth()) {
    player.position.x = game.getWindowWidth() - player.width;
}
if(player.position.y + player.height > game.getWindowHeight()) {
    player.position.y = game.getWindowHeight() - player.height;
}

if (!Game::getInstance().getAutoShoot() && keyborad[SDL_SCANCODE_J]){
        auto currentTime = SDL_GetTicks();
        if (currentTime - player.lastShootTime > player.coolDown){
            shootPlayer();
            player.lastShootTime = currentTime;
        }
    }
if(Game::getInstance().getAutoShoot()){
    auto currentTime = SDL_GetTicks();
        if (currentTime - player.lastShootTime > player.coolDown){
            shootPlayer();
            player.lastShootTime = currentTime;
        }
}

}

void SceneMain::shootPlayer()
{
    if(isDead) return;
    // 创建新子弹
    auto projectile = new ProjectilePlayer(projectilePlayerTemplate);
    // 定位在飞机顶部中央
    projectile->position.x = player.position.x + player.width / 2 - projectile->width / 2;
    projectile->position.y = player.position.y;
    // 添加到活动子弹列表
    projectilesPlayer.push_back(projectile);
}

void SceneMain::useSkill()
{
    if(isDead) return;
    player.hasSkill = false;
    player.underSkill = true;
    player.lastSkillTime = SDL_GetTicks();

    // 左前方子弹（projectilePlayerTemplate1）
    for(float i = 3.5; i< 8;i+=0.5){
    auto projectile1 = new ProjectilePlayer(projectilePlayerTemplate1);
    projectile1->position.x = player.position.x + player.width / 2 - projectile1->width / 2;
    projectile1->position.y = player.position.y;
    // 方向：左前方，约30度偏移
    float angle1 = -M_PI / i;  // -30度
    projectile1->direction.x = sin(angle1);
    projectile1->direction.y = -cos(angle1);
    projectilesPlayer.push_back(projectile1);

    // 右前方子弹（projectilePlayerTemplate2）
    auto projectile2 = new ProjectilePlayer(projectilePlayerTemplate2);
    projectile2->position.x = player.position.x + player.width / 2 - projectile2->width / 2;
    projectile2->position.y = player.position.y;
    // 方向：右前方，约30度偏移
    float angle2 = M_PI / i;  // +30度
    projectile2->direction.x = sin(angle2);
    projectile2->direction.y = -cos(angle2);
    projectilesPlayer.push_back(projectile2);
    }
}

void SceneMain::updatePlayerProjectiles(float deltaTime)
{
    int margin = 32; // 子弹超出屏幕外边界的距离
    for (auto it = projectilesPlayer.begin(); it != projectilesPlayer.end();){
        auto projectile = *it;
        // 更新子弹位置（根据方向向量）
        projectile->position.x += projectile->speed * projectile->direction.x * deltaTime;
        projectile->position.y += projectile->speed * projectile->direction.y * deltaTime;
        // 检查子弹是否超出屏幕
        if (projectile->position.y + margin < 0){
            delete projectile;
            it = projectilesPlayer.erase(it);
        }else {
            bool ishit = false;
            for(auto enemy : enemies){
                SDL_Rect enemyRect = {
                    static_cast<int>(enemy->position.x),
                    static_cast<int>(enemy->position.y),
                    enemy->width,
                    enemy->height
                };
                SDL_Rect projectileRect = {
                    static_cast<int>(projectile->position.x),
                    static_cast<int>(projectile->position.y),
                    projectile->width,
                    projectile->height
                };
                if(SDL_HasIntersection(&enemyRect, &projectileRect)){
                    ishit = true;
                    enemy->currentHealth -= projectile->damage;
                    delete projectile;
                    it = projectilesPlayer.erase(it);
                    break;
                }
        }
        if(!ishit){
            ++it;
        }
    }
}
}
void SceneMain::renderPlayerProjectiles()
{
    for (auto projectile : projectilesPlayer){
        SDL_Rect projectileRect = {
            static_cast<int>(projectile->position.x),
            static_cast<int>(projectile->position.y),
            projectile->width,
            projectile->height
        };
        SDL_RenderCopy(game.getRenderer(), projectile->texture, NULL, &projectileRect);
    }
}

void SceneMain::spawEnemy()
{
    // 时停期间不生成敌机
    if (isTimeStopped) {
        return;
    }

    // 利用随机数决定是否生成敌机（每帧约1/60的概率）
    if (dis(gen) > 1 / 60.0f){
        return;
    }
    
    // 创建新敌机，随机生成二者之一
    Enemy* enemy = dis(gen) > 0.5f ? new Enemy(enemyTemplate1) : new Enemy(enemyTemplate2);
    
    // 利用随机数决定敌机的水平位置
    enemy->position.x = dis(gen) * (game.getWindowWidth() - enemy->width);
    
    // 垂直位置设在屏幕上方（刚好不可见）
    enemy->position.y = - enemy->height;
    
    // 添加到活动敌机列表
    enemies.push_back(enemy);
}

void SceneMain::updateEnemies(float deltaTime)
{
    auto currentTime = SDL_GetTicks();
    for (auto it = enemies.begin(); it != enemies.end();){
        auto enemy = *it;
        
        // 时停期间敌机不移动
        if (!isTimeStopped) {
            enemy->position.y += enemy->speed * deltaTime;
        }
        
        // 检查敌机是否飞出屏幕底部
        if (enemy->position.y > game.getWindowHeight()){
            delete enemy;
            it = enemies.erase(it);
        }else {
            if(!isTimeStopped && currentTime - enemy->lastShootTime > enemy->coolDown && !isDead){
                shootEnemy(enemy);
                enemy->lastShootTime = currentTime;
            }
            if (enemy->currentHealth <= 0){
                enemyExplode(enemy);
                it = enemies.erase(it);
            } else {
                ++it;
            }
        }
        
    }
}

void SceneMain::renderEnemies()
{
    for (auto enemy : enemies){
        SDL_Rect enemyRect = {
            static_cast<int>(enemy->position.x),
            static_cast<int>(enemy->position.y),
            enemy->width,
            enemy->height
        };
        SDL_RenderCopy(game.getRenderer(), enemy->texture, NULL, &enemyRect);
    }
}

void SceneMain::shootEnemy(Enemy *enemy)
{
    // 创建新子弹
    auto projectile = enemy->speed == 200? new ProjectileEnemy(projectileEnemyTemplate1) : (dis(gen) > 0.82f ? new ProjectileEnemy(projectileEnemyTemplate2) : new ProjectileEnemy(projectileEnemyTemplate3));
    
    // 设置子弹初始位置（从敌机中央发射）
    projectile->position.x = enemy->position.x + enemy->width / 2 - projectile->width / 2;
    projectile->position.y = enemy->position.y + enemy->height / 2 - projectile->height / 2;
    
    // 计算子弹飞向玩家的方向
    projectile->direction = getDirection(enemy);
    
    // 添加到敌机子弹列表
    projectilesEnemy.push_back(projectile);
}

SDL_FPoint SceneMain::getDirection(Enemy *enemy)
{
    // 计算从敌机中心到玩家中心的向量
    auto x = (player.position.x + player.width / 2) - (enemy->position.x + enemy->width / 2);
    auto y = (player.position.y + player.height / 2) - (enemy->position.y + enemy->height / 2);
    
    // 计算向量长度
    auto length = sqrt(x * x + y * y);
    
    // 归一化向量
    x /= length;
    y /= length;
    
    return SDL_FPoint{x, y};
}

void SceneMain::updateEnemyProjectiles(float deltaTime)
{
    auto margin = 32; // 屏幕边缘额外余量
    for (auto it = projectilesEnemy.begin(); it != projectilesEnemy.end();){
        auto projectile = *it;
        
        // 根据方向向量更新子弹位置
        projectile->position.x += projectile->speed * projectile->direction.x * deltaTime;
        projectile->position.y += projectile->speed * projectile->direction.y * deltaTime;
        
        // 检查子弹是否飞出屏幕边界
        if (projectile->position.y > game.getWindowHeight() + margin ||
            projectile->position.y < - margin ||
            projectile->position.x < - margin ||
            projectile->position.x > game.getWindowWidth() + margin){
            delete projectile;
            it = projectilesEnemy.erase(it);
        }else {
            SDL_Rect playerRect = {
                static_cast<int>(player.position.x),
                static_cast<int>(player.position.y),
                player.width,
                player.height
            };
            SDL_Rect projectileRect = {
                static_cast<int>(projectile->position.x),
                static_cast<int>(projectile->position.y),
                projectile->width,
                projectile->height
            };
            if (SDL_HasIntersection(&playerRect, &projectileRect) && !isDead){
                if (!player.hasShield) {
                    player.currentHealth -= projectile->damage;
                    player.hasShield = true;
                    player.shieldTime = 3.0f;
                }
                delete projectile;
                it = projectilesEnemy.erase(it);
            }
            else ++it;
        }
    }
}

void SceneMain::renderEnemyProjectiles()
{
    for (auto projectile : projectilesEnemy){
        SDL_Rect projectileRect = {
            static_cast<int>(projectile->position.x),
            static_cast<int>(projectile->position.y),
            projectile->width,
            projectile->height
        };
        
        // 计算子弹旋转角度（从弧度转换为角度，并调整为正确的方向）
        float angle = atan2(projectile->direction.y, projectile->direction.x) * 180 / M_PI - 90;
        
        // 使用带旋转参数的渲染函数
        SDL_RenderCopyEx(game.getRenderer(), projectile->texture, NULL, &projectileRect, angle, NULL, SDL_FLIP_NONE);
    }
}

void SceneMain::updatePlayer(float deltaTime)
{
    if (isDead) {
        player.hasShield = false;
        player.shieldTime = 0.0f;
        return;
    }

    // 更新护盾状态
    if (player.hasShield) {
        player.shieldTime -= deltaTime;
        if (player.shieldTime <= 0.0f) {
            player.shieldTime = 0.0f;
            player.hasShield = false;
        }
    }

    // 更新时停状态
    if (isTimeStopped) {
        timeStopTime -= deltaTime;
        if (timeStopTime <= 0.0f) {
            timeStopTime = 0.0f;
            isTimeStopped = false;
        }
    }

    // 更新技能状态
    if (player.underSkill) {
        // 技能持续期间按冷却间隔发射两侧子弹
        auto currentTime = SDL_GetTicks();
        if (currentTime - player.lastShootTime > player.coolDown) {
            player.skillTime -= 1;
            for(float i = 9 ;i< 21;i+=2.5){
    auto projectile1 = new ProjectilePlayer(projectilePlayerTemplate1);
    projectile1->position.x = player.position.x + player.width / 2 - projectile1->width / 2;
    projectile1->position.y = player.position.y;
    // 方向：左前方，约30度偏移
    float angle1 = -M_PI / i;  // -30度
    projectile1->direction.x = sin(angle1);
    projectile1->direction.y = -cos(angle1);
    projectilesPlayer.push_back(projectile1);

    // 右前方子弹（projectilePlayerTemplate2）
    auto projectile2 = new ProjectilePlayer(projectilePlayerTemplate2);
    projectile2->position.x = player.position.x + player.width / 2 - projectile2->width / 2;
    projectile2->position.y = player.position.y;
    // 方向：右前方，约30度偏移
    float angle2 = M_PI / i;  // +30度
    projectile2->direction.x = sin(angle2);
    projectile2->direction.y = -cos(angle2);
    projectilesPlayer.push_back(projectile2);
    }
            // 左前方子弹
            for(float i = 20; i< 100;i+=10){
    auto projectile1 = new ProjectilePlayer(projectilePlayerTemplate1);
    projectile1->position.x = player.position.x + player.width / 2 - projectile1->width / 2;
    projectile1->position.y = player.position.y;
    // 方向：左前方，约30度偏移
    float angle1 = -M_PI / i;  // -30度
    projectile1->direction.x = sin(angle1);
    projectile1->direction.y = -cos(angle1);
    projectilesPlayer.push_back(projectile1);

    // 右前方子弹（projectilePlayerTemplate2）
    auto projectile2 = new ProjectilePlayer(projectilePlayerTemplate2);
    projectile2->position.x = player.position.x + player.width / 2 - projectile2->width / 2;
    projectile2->position.y = player.position.y;
    // 方向：右前方，约30度偏移
    float angle2 = M_PI / i;  // +30度
    projectile2->direction.x = sin(angle2);
    projectile2->direction.y = -cos(angle2);
    projectilesPlayer.push_back(projectile2);
    }
        for(float i = 3.5; i< 8;i+=1){
    auto projectile1 = new ProjectilePlayer(projectilePlayerTemplate1);
    projectile1->position.x = player.position.x + player.width / 2 - projectile1->width / 2;
    projectile1->position.y = player.position.y;
    // 方向：左前方，约30度偏移
    float angle1 = -M_PI / i;  // -30度
    projectile1->direction.x = sin(angle1);
    projectile1->direction.y = -cos(angle1);
    projectilesPlayer.push_back(projectile1);

    // 右前方子弹（projectilePlayerTemplate2）
    auto projectile2 = new ProjectilePlayer(projectilePlayerTemplate2);
    projectile2->position.x = player.position.x + player.width / 2 - projectile2->width / 2;
    projectile2->position.y = player.position.y;
    // 方向：右前方，约30度偏移
    float angle2 = M_PI / i;  // +30度
    projectile2->direction.x = sin(angle2);
    projectile2->direction.y = -cos(angle2);
    projectilesPlayer.push_back(projectile2);
    }

            player.lastShootTime = currentTime;
        }
        // 技能持续时间结束
        if (player.skillTime <= 0) {
            player.skillTime = 0;
            player.underSkill = false;
            player.lastSkillTime = SDL_GetTicks();
            player.skillTime = 3;  // 重置技能持续时间
        }
    }

    // 技能冷却恢复
    if (!player.hasSkill && !player.underSkill) {
        auto currentTime = SDL_GetTicks();
        if ((currentTime - player.lastSkillTime) / 1000.0f >= player.skillCD) {
            player.hasSkill = true;
        }
    }

    if (player.currentHealth <= 0){
        // 玩家死亡处理
        isDead = true;
        playerExplode(&player);
    }
    for (auto enemy : enemies){
        SDL_Rect enemyRect = {
            static_cast<int>(enemy->position.x),
            static_cast<int>(enemy->position.y),
            enemy->width,
            enemy->height
        };
        SDL_Rect playerRect = {
            static_cast<int>(player.position.x),
            static_cast<int>(player.position.y),
            player.width,
            player.height
        };
        if (SDL_HasIntersection(&playerRect, &enemyRect)){
            if (!player.hasShield) {
                player.currentHealth -= 1;
                player.hasShield = true;
        player.shieldTime = 3.0f;
            }
            enemy->currentHealth = 0;
        }
    }
}

void SceneMain::enemyExplode(Enemy *enemy)
{
    auto currentTime = SDL_GetTicks();
    auto explosion = new Explosion(explosionTemplate);
    explosion->position.x = enemy->position.x + enemy->width / 2 - explosion->width / 2;
    explosion->position.y = enemy->position.y + enemy->height / 2 - explosion->height / 2;
    explosion->startTime = currentTime;
    explosions.push_back(explosion);
    if (dis(gen) < 0.2f){
        dropItem(enemy);
    }
    delete enemy;
}

void SceneMain::playerExplode(Player *player){
    auto currentTime = SDL_GetTicks();
    auto explosion = new Explosion(explosionTemplate);
    explosion->position.x = player->position.x + player->width / 2 - explosion->width / 2;
    explosion->position.y = player->position.y + player->height / 2 - explosion->height / 2;
    explosion->startTime = currentTime;
    explosions.push_back(explosion);

}

void SceneMain::updateExplosions(float)
{
    auto currentTime = SDL_GetTicks();
    for (auto it = explosions.begin(); it != explosions.end();)
    {
        auto explosion = *it;
        // 根据时间差和帧率计算当前帧索引
        explosion->currentFrame = (currentTime - explosion->startTime) * explosion->FPS / 1000;
        // 检查动画是否播放完毕
        if (explosion->currentFrame >= explosion->totlaFrame){
            delete explosion;
            it = explosions.erase(it);
        }else {
            ++it;
        }
    }
}

void SceneMain::renderExplosions()
{
    for (auto explosion : explosions)
    {
        // 定义源矩形（精灵图上的区域）
        SDL_Rect src = {explosion->currentFrame * explosion->width, 0, explosion->width, explosion->height};
        // 定义目标矩形（屏幕上的位置）
        SDL_Rect dst = {
            static_cast<int>(explosion->position.x), 
            static_cast<int>(explosion->position.y), 
            explosion->width, 
            explosion->height
        };
        // 绘制当前帧
        SDL_RenderCopy(game.getRenderer(), explosion->texture, &src, &dst);
    }
}
void SceneMain::dropItem(Enemy *enemy)
{
    auto item1 = new Item(itemLifeTemplate);
    auto item2 = new Item(itemShieldTemplate);
    auto item3 = new Item(itemTimeTemplate);
    auto item = dis(gen) < 0.33f ? item1 : dis(gen) < 0.66f ? item2 : item3;
    item->position.x = enemy->position.x + enemy->width / 2 - item->width / 2;
    item->position.y = enemy->position.y + enemy->height / 2 - item->height / 2;
    
    // 生成随机方向
    float angle = dis(gen) * 2 * M_PI;
    item->direction.x = cos(angle);
    item->direction.y = sin(angle);
    
    items.push_back(item);
}
void SceneMain::updateItems(float deltaTime)
{
    for (auto it = items.begin(); it != items.end();)
    {
        auto item = *it;
        // 更新位置
        item->position.x += item->direction.x * item->speed * deltaTime;
        item->position.y += item->direction.y * item->speed * deltaTime;
        
        // 处理屏幕边缘反弹
        if (item->position.x < 0 && item->bounceCount > 0) {
            item->direction.x = -item->direction.x;
            item->bounceCount--;
        }
        if (item->position.x + item->width > game.getWindowWidth() && item->bounceCount > 0) {
            item->direction.x = -item->direction.x;
            item->bounceCount--;
        }
        if (item->position.y < 0 && item->bounceCount > 0) {
            item->direction.y = -item->direction.y;
            item->bounceCount--;
        }
        if (item->position.y + item->height > game.getWindowHeight() && item->bounceCount > 0) {
            item->direction.y = -item->direction.y;
            item->bounceCount--;
        }
        
        // 如果超出屏幕范围则删除
        if (item->position.x + item->width < 0 || 
            item->position.x > game.getWindowWidth() ||
            item->position.y + item->height < 0 || 
            item->position.y > game.getWindowHeight()){
            delete item;
            it = items.erase(it);
        }
        else{
            // 检测物品与玩家的碰撞
            SDL_Rect itemRect = {
                static_cast<int>(item->position.x), 
                static_cast<int>(item->position.y), 
                item->width, 
                item->height
            };
            SDL_Rect playerRect = {
                static_cast<int>(player.position.x), 
                static_cast<int>(player.position.y), 
                player.width, 
                player.height
            };
            
            if (SDL_HasIntersection(&itemRect, &playerRect))
            {
                playerGetItem(item);
                delete item;
                it = items.erase(it);
            }else
            {
                ++it;
            }
        }
    }
}

void SceneMain::playerGetItem(Item *item)
{
    if (item->type == ItemType::Life){
        player.currentHealth += 1;
        if (player.currentHealth > player.maxHealth){
            player.currentHealth = player.maxHealth;
        }
    } else if (item->type == ItemType::Shield){
        player.hasShield = true;
        player.shieldTime = 5.0f;  // 护盾持续5秒，已有护盾时刷新时间
    } else if (item->type == ItemType::Time){
        isTimeStopped = true;
        timeStopTime = 3.0f;  // 时停持续5秒，已有时停时刷新时间
        // 删除所有敌机子弹
        for (auto projectile : projectilesEnemy){
            delete projectile;
        }
        projectilesEnemy.clear();
    }
}
void SceneMain::renderItems()
{
    for (auto &item : items)
    {
        SDL_Rect itemRect = {
            static_cast<int>(item->position.x), 
            static_cast<int>(item->position.y), 
            item->width, 
            item->height
        };
        SDL_RenderCopy(game.getRenderer(), item->texture, NULL, &itemRect);
    }   
}