#include "EnemySlime.h"
#include "Input/Input.h"

#include "Graphics/Graphics.h"
#include "SceneTitle.h"
#include "SceneManager.h"
#include "SceneLoading.h"
#include "SceneScore.h"

#include "ScoreDataManager.h"

//�R���X�g���N�^
EnemySlime::EnemySlime()
{
    //model = new Model("Data/Model/Pizza/Pizza.mdl");
    //model = new Model("Data/Model/Pizza/RealPizza.mdl");
    model = new Model("Data/Model/PIZZA/PIZZA.mdl");

    //���f�����傫���̂ŃX�P�[�����O
    scale.x = scale.y = scale.z = 0.001f;

    //���A�����ݒ�
    radius = 0.5f;
    height = 1.0f;
}

//�f�X�g���N�^
EnemySlime::~EnemySlime()
{
    delete model;
}

//�X�V����
void EnemySlime::Update(float elapsedTime)
{
    //���͏����X�V
    UpdateVelocity(elapsedTime);

    //�I�u�W�F�N�g�s��X�V
    UpdateTransform();

    //���f���s��X�V
    model->UpdateTransform(transform);

    EnemyMove();

}

void EnemySlime::EnemyMove()
{

    Mouse& gameMouse = Input::Instance().GetMouse();

    int ax = gameMouse.GetPositionX();
    int ay = gameMouse.GetPositionY();
    //////////////////////////////
    if (gameMouse.GetButton() & Mouse::BTN_LEFT)
    {
        switch (state)
        {
        case 0:
            if (MousePoint == 0 && ax > 800 && ax < 950 && ay > 400 && ay < 550)
            {    
                MousePoint += 1;
                ++state;
            }
        case 1:
            if (MousePoint == 1 && ax > 950 && ax < 1100 && ay > 400 && ay < 550)
            {
                MousePoint += 1;
                ++state;
            }
        case 2:
            if (MousePoint == 2 && ax > 950 && ax < 1100 && ay > 550 && ay < 700)
            {
                MousePoint += 1;
                ++state;
            }
        case 3:
            if (MousePoint == 3 && ax > 800 && ax < 950 && ay > 550 && ay < 700)
            {
                    PizzaPos += Pizzaspeed;
           
                if (PizzaPos > powerMax)
                {
                    PizzaPos -= Pizzaspeed;
                    MousePoint = 0;
                    state = 0;
                }
                
                scale.x += scaleup;
                scale.z += scaleup;

                MousePoint = 0;
                state = 0;

                if (gameMouse.GetButtonUp() & Mouse::BTN_LEFT)
                {
                    scale.x = scale.x;
                    scale.z += scale.z;
                }

                if (scale.x > scaleMax && scale.z > scaleMax)
                {
                    scale.x = scaleMax;
                    scale.z = scaleMax;
                }
            }
        }

    }
   
    //�s�U���΂�
    if (!(gameMouse.GetButton() & Mouse::BTN_LEFT))
    {
        EnemyHight();
    }

    if (speed == 0) ++scoretimer;

    if (scoretimer > 120) SceneManager::Instance().ChangeScene(new SceneScore);

}

void EnemySlime::EnemyHight()
{
    //position��speed�𑫂�
    if (PizzaPos > 0.0f)
    {
        position.y += speed;
    }

    if (position.y >= PizzaPos && PizzaPos != 0.0f)
    {
        speed = 0;

        scaleup = 0;

        ScoreDataManager::Instance().SetPos(position.y);
    }

}

//�`�揈��
void EnemySlime::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
}

