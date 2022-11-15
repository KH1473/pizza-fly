#pragma once

#include "Graphics/Model.h"
#include "Enemy.h"
#include "Audio/Audio.h"

//�X���C��
class EnemySlime : public Enemy
{
public:
    EnemySlime();
    ~EnemySlime() override;

    //�X�V����
    void Update(float elapsedTime) override;

    void EnemyMove();

    void EnemyHight();
    
    //�`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* shader) override;
    
private:
    Model* model = nullptr;
    
    //�s�U����ԍ���
    float PizzaPos = 0.0f;

    //�����̍ŏ��l
    float powerMin = 0.0f;
    //�����̍ő�l
    float powerMax = 5000.0f;

    //1��������ɉ��Z���鍂���̑傫��
    float pizzaRising = 50.00000f;

    //�X�P�[������
    float scaleup = 0.0001f;

    //�X�P�[���̍ő�l
    float scaleMax = 0.1f;
    
    //��ԑ���
    float speed = 0.51213f;

    int     MousePoint = 0;
    int     state = 0;
    int     scoretimer = 0;
    int     GameFlag = 0;

    //�s�U�̉�]��
    std::unique_ptr<AudioSource> pizzaangle_bgm = nullptr;
};