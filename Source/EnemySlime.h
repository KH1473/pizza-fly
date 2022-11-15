#pragma once

#include "Graphics/Model.h"
#include "Enemy.h"
#include "Audio/Audio.h"

//スライム
class EnemySlime : public Enemy
{
public:
    EnemySlime();
    ~EnemySlime() override;

    //更新処理
    void Update(float elapsedTime) override;

    void EnemyMove();

    void EnemyHight();
    
    //描画処理
    void Render(ID3D11DeviceContext* dc, Shader* shader) override;
    
private:
    Model* model = nullptr;
    
    //ピザが飛ぶ高さ
    float PizzaPos = 0.0f;

    //高さの最小値
    float powerMin = 0.0f;
    //高さの最大値
    float powerMax = 5000.0f;

    //1周あたりに加算する高さの大きさ
    float pizzaRising = 50.00000f;

    //スケール増加
    float scaleup = 0.0001f;

    //スケールの最大値
    float scaleMax = 0.1f;
    
    //飛ぶ速さ
    float speed = 0.51213f;

    int     MousePoint = 0;
    int     state = 0;
    int     scoretimer = 0;
    int     GameFlag = 0;

    //ピザの回転音
    std::unique_ptr<AudioSource> pizzaangle_bgm = nullptr;
};