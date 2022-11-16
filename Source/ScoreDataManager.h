#pragma once

#include <string>

class ScoreDataManager
{
private:
    ScoreDataManager() {}
    ~ScoreDataManager() {}

private:
    // スコアとなるピザの位置
    float PizzaPos;

    //ピザのscale値
    DirectX::XMFLOAT3 PizzaScale{ 0, 0, 0 };

    //タイマー
    float angleTimerCount;

    // 利用されるピザのモデルファイル名
    std::string pizzaModelFilename;

public:
    static ScoreDataManager& Instance() {
        static ScoreDataManager inst;
        return inst;
    }

    //高さ取得
    void SetPos(float pizzaPos) { PizzaPos = pizzaPos; }
    float GetPos() { return PizzaPos; }

    //大きさの取得
    void SetScale(DirectX::XMFLOAT3 pizzaScale) { PizzaScale = pizzaScale; }
    DirectX::XMFLOAT3 GetScale() { return PizzaScale; }

    //秒数取得
    void SetTimer(float angleTimer) { angleTimerCount = angleTimer; }
    int GetTimer() { return angleTimerCount; }

    void SetPizzaModelFilename(std::string n) { pizzaModelFilename = n; }
    std::string GetPizzaModelFilename() { return pizzaModelFilename; }

};
