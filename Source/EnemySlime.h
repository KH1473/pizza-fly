#pragma once

#include "Graphics/Model.h"
#include "Enemy.h"
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
    
    int scoreP = 0;

private:
    Model* model = nullptr;
    
    //ピザが飛ぶ高さ
    float PizzaPos = 0.0f;

    //高さの最小値
    float powerMin = 0.0f;
    //高さの最大値
    float powerMax = 3000.0f;

    //1周あたりに加算する高さの大きさ
    float Pizzaspeed = 50.0f;

    //スケール増加
    float scaleup = 0.0001f;

    //スケールの最大値
    float scaleMax = 0.1f;
    
    //飛ぶ速さ
    float speed = 0.5f;

    int     MousePoint = 0;
    int     state = 0;
    int scoretimer = 0;
};