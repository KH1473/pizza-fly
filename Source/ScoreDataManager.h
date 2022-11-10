#pragma once

class ScoreDataManager
{
private:
    ScoreDataManager() {}
    ~ScoreDataManager() {}

    float PizzaPos;
    DirectX::XMFLOAT3 PizzaScale;
public:
    static ScoreDataManager& Instance() {
        static ScoreDataManager inst;
        return inst;
    }

    void SetPos(float pizzaPos) { PizzaPos = pizzaPos; }
    float GetPos() { return PizzaPos; }
    void SetScale(DirectX::XMFLOAT3 pizzaScale) { PizzaScale = pizzaScale; }
    DirectX::XMFLOAT3 GetScalse() { return PizzaScale; }
};
