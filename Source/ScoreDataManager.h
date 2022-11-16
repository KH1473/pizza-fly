#pragma once

#include <string>

class ScoreDataManager
{
private:
    ScoreDataManager() {}
    ~ScoreDataManager() {}

private:
    // �X�R�A�ƂȂ�s�U�̈ʒu
    float PizzaPos;

    //�s�U��scale�l
    DirectX::XMFLOAT3 PizzaScale{ 0, 0, 0 };

    //�^�C�}�[
    float angleTimerCount;

    // ���p�����s�U�̃��f���t�@�C����
    std::string pizzaModelFilename;

public:
    static ScoreDataManager& Instance() {
        static ScoreDataManager inst;
        return inst;
    }

    //�����擾
    void SetPos(float pizzaPos) { PizzaPos = pizzaPos; }
    float GetPos() { return PizzaPos; }

    //�傫���̎擾
    void SetScale(DirectX::XMFLOAT3 pizzaScale) { PizzaScale = pizzaScale; }
    DirectX::XMFLOAT3 GetScale() { return PizzaScale; }

    //�b���擾
    void SetTimer(float angleTimer) { angleTimerCount = angleTimer; }
    int GetTimer() { return angleTimerCount; }

    void SetPizzaModelFilename(std::string n) { pizzaModelFilename = n; }
    std::string GetPizzaModelFilename() { return pizzaModelFilename; }

};
