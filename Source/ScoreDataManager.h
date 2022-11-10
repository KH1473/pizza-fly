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

    // 利用されるピザのモデルファイル名
    std::string pizzaModelFilename;

public:
    static ScoreDataManager& Instance() {
        static ScoreDataManager inst;
        return inst;
    }

    void SetPos(float pizzaPos) { PizzaPos = pizzaPos; }
    float GetPos() { return PizzaPos; }

    void SetPizzaModelFilename(std::string n) { pizzaModelFilename = n; }
    std::string GetPizzaModelFilename() { return pizzaModelFilename; }

};
