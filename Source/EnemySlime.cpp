#include "EnemySlime.h"
#include "Input/Input.h"

#include "Graphics/Graphics.h"
#include "SceneTitle.h"
#include "SceneManager.h"
#include "SceneLoading.h"
#include "SceneScore.h"

#include "PizzaScore.h"

#include "PizzaConstants.h"

#include "ScoreDataManager.h"

//�R���X�g���N�^
EnemySlime::EnemySlime()
{
    model = new Model(ScoreDataManager::Instance().GetPizzaModelFilename().c_str());
    
    //model = new Model(KOGE_MODEL);
    SceneType sceneType = SceneManager::Instance().GetSceneType();

    if (sceneType == SceneType::Main)
    {
        //���f�����傫���̂ŃX�P�[�����O
        scale.x = scale.y = scale.z = 0.001f;
        ScoreDataManager::Instance().SetScale(scale);
    }
    else if (sceneType == SceneType::Score)
    {
        //scale�l�ۑ�       
        ScoreDataManager::Instance().GetScale();
        scale.x = scale.y = scale.z = 0.005f;
    }

    //���A�����ݒ�
    radius = 0.5f;
    height = 1.0f;

    //Effect
    Barrier = new Effect("Data/Effect/barrier/barrier.efkefc");
    Fire = new Effect("Data/Effect/Fire/fire2.efkefc");

    //�s�U�̉�]��
    pizzaangle_bgm = Audio::Instance().LoadAudioSource("Data/Audio/pizza��].wav");
}

//�f�X�g���N�^
EnemySlime::~EnemySlime()
{
    delete model;

    if (Fire != nullptr)
    {
        delete Fire;
        Fire = nullptr;
    }

    if (Barrier != nullptr)
    {
        delete Barrier;
        Barrier = nullptr;
    }
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
    
   EnemyMove(elapsedTime);

}

void EnemySlime::EnemyMove(float elapsedTime)
{
    SceneType sceneType = SceneManager::Instance().GetSceneType();
    
    ScoreDataManager::Instance().SetTimer(angleTimer);

    if (sceneType == SceneType::Main)
    {
        Mouse& gameMouse = Input::Instance().GetMouse();

        int ax = gameMouse.GetPositionX();
        int ay = gameMouse.GetPositionY();
        //////////////////////////////
        if (gameMouse.GetButton() & Mouse::BTN_LEFT && GameFlag == 0)
        {
            switch (state)
            {
            case 0:
                if (MousePoint == 0 && ax > 800 && ax < 950 && ay > 400 && ay < 550)
                {
                    MousePoint += 1;
                    ++state;
                    
                    pizzaangle_bgm->Play(false);

                    DirectX::XMFLOAT3 effectplay = position;
                    //effectplay.y += position.y * 0.5f;
                    Fire->Play(effectplay);
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
                    PizzaPos += pizzaRising;

                    if (position.y <= 2.0f)
                    {
                        scale.x += scaleup;
                        scale.z += scaleup;
                    }

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

            angleTimer -= elapsedTime;
        }

        //�s�U���΂�
        if(angleTimer < 0.5f)
        {
            angleTimer = 0.0f;
            hightTimer++;
        }

        if (hightTimer > 0)
        {
            ScoreDataManager::Instance().SetPos(position.y);
            ScoreDataManager::Instance().SetScale(scale);
            EnemyHight();
        }

        if (speed == 0) {
            ScoreDataManager::Instance().SetPos(position.y);
            ScoreDataManager::Instance().SetScale(scale);
            ++scoretimer;

            gameend = Audio::Instance().LoadAudioSource("Data/Audio/game_end.wav");
            if (scoretimer < 60)
            gameend->Play(false);
        }

        if (scoretimer > 120) SceneManager::Instance().ChangeScene(new PizzaScore);
    }
   
}

void EnemySlime::EnemyHight()
{
    //position��speed�𑫂�
    if (PizzaPos > 0.0f)
    {
        position.y += speed;
    }

    DirectX::XMFLOAT3 effectplay = position;
    if (position.y > 30.0f)
    {
        Barrier->Play(effectplay);
    }
    
    if (position.y >= PizzaPos && PizzaPos != 0.0f)
    {
        speed = 0;

        scaleup = 0;
    }
}

//�`�揈��
void EnemySlime::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
}

