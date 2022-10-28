#pragma once

class ScoreDataManager
{
private:
    ScoreDataManager() {}
    ~ScoreDataManager() {}

    float PizzaPos;
public:
    static ScoreDataManager& Instance() {
        static ScoreDataManager inst;
        return inst;
    }

    void SetPos(float pizzaPos) { PizzaPos = pizzaPos; }
    float GetPos() { return PizzaPos; }
};
