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

//コンストラクタ
EnemySlime::EnemySlime()
{
    model = new Model(ScoreDataManager::Instance().GetPizzaModelFilename().c_str());
    
    //model = new Model(KOGE_MODEL);
    SceneType sceneType = SceneManager::Instance().GetSceneType();

    if (sceneType == SceneType::Main)
    {
        //モデルが大きいのでスケーリング
        scale.x = scale.y = scale.z = 0.001f;
    }
    else if (sceneType == SceneType::Score)
    {
        //モデルが大きいのでスケーリング
        scale.x = scale.y = scale.z = 0.005f;
    }

    //幅、高さ設定
    radius = 0.5f;
    height = 1.0f;

    //ピザの回転音
    pizzaangle_bgm = Audio::Instance().LoadAudioSource("Data/Audio/pizza回転.wav");
}

//デストラクタ
EnemySlime::~EnemySlime()
{
    delete model;
    
}

//更新処理
void EnemySlime::Update(float elapsedTime)
{
    //遠力処理更新
    UpdateVelocity(elapsedTime);

    //オブジェクト行列更新
    UpdateTransform();

    //モデル行列更新
    model->UpdateTransform(transform);
    
   EnemyMove();

}

void EnemySlime::EnemyMove()
{
    SceneType sceneType = SceneManager::Instance().GetSceneType();

    if (sceneType == SceneType::Main)
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
                    
                    pizzaangle_bgm->Play(false);
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

        //ピザを飛ばす
        if (!(gameMouse.GetButton() & Mouse::BTN_LEFT))
        {
            EnemyHight();
        }

        if (speed == 0) {
            ScoreDataManager::Instance().SetPos(position.y);
            ++scoretimer;
        }

        if (scoretimer > 120) SceneManager::Instance().ChangeScene(new PizzaScore);
    }
    else if (sceneType == SceneType::Score)
    {

    }
}

void EnemySlime::EnemyHight()
{


    //positionにspeedを足す
    if (PizzaPos > 0.0f)
    {
        position.y += speed;
    }

    if (position.y >= PizzaPos && PizzaPos != 0.0f)
    {
        speed = 0;

        scaleup = 0;
    }

}

//描画処理
void EnemySlime::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
}

