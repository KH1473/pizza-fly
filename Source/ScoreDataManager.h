#pragma once

class ScoreDataManager
{
private:
    ScoreDataManager() {}
    ~ScoreDataManager() {}

    // �X�R�A�ƂȂ�s�U�̈ʒu
    float PizzaPos;

    // ���p�����s�U�̃��f���t�@�C����
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
