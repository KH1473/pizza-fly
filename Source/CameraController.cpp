#include "CameraController.h"
#include "Camera.h"
#include "Input/Input.h"

//#include <stdlib.h>
#include "SceneManager.h"

//更新処理
void CameraController::Update(float elapsedTime)
{

    Mouse& gameMouse = Input::Instance().GetMouse();

    float ax = 0.0f;
    float ay = 0.0f;

    int mouseX = gameMouse.GetPositionX();
    int mouseY = gameMouse.GetPositionY();

    SceneType sceneType = SceneManager::Instance().GetSceneType();

    if (sceneType == SceneType::Main)
    {
        switch (cameracount)
        {
        case 0:

            if (800 <= mouseX && mouseX < 1100 && mouseY >= 400 && mouseY < 700)
            {
                if (gameMouse.GetButton() & Mouse::BTN_LEFT)
                {

                    if (ax >= 0.0f)
                    {
                        ax -= 1.0f;
                    }
                    else if (ax <= -10.0f)
                    {
                        ax += 1.0f;
                    }

                    if (ay <= 0.0f)
                    {
                        ay += 0.5f;
                    }
                    else if (ay >= 5.0f)
                    {
                        ay -= 0.5f;
                    }

                }
                else if (gameMouse.GetButtonUp() & Mouse::BTN_LEFT)
                {
                    cameracount++;
                }
            }

            break;
        case 1:
            cameracount = 1;
            break;
        }
    }
    else if (sceneType == SceneType::Score)
    {
        if (ax >= 0.0f)
        {
            ax -= 1.0f;
        }
        else if (ax <= -10.0f)
        {
            ax += 1.0f;
        }

        if (ay <= 0.0f)
        {
            ay += 0.5f;
        }
        else if (ay >= 5.0f)
        {
            ay -= 0.5f;
        }
    }

    //カメラの回転速度
    float speed = rollSpeed * elapsedTime;

    //スティックの入力値に合わせてX軸とY軸を回転
    angle.x += ay * speed;
    angle.y += ax * speed;

    //X軸のカメラ回転を制限
    if (angle.x >= maxAngleX)
    {
        angle.x = maxAngleX;
    }
    if (angle.x <= minAngleX)
    {
        angle.x = minAngleX;
    }

    //Y軸の回転値を-3.14~3.14に収まるようにする
    if (angle.y < -DirectX::XM_PI)
    {
        angle.y += DirectX::XM_2PI;
    }
    if (angle.y > DirectX::XM_PI)
    {
        angle.y -= DirectX::XM_2PI;
    }
    //カメラ回転値を回転行列に変換
    DirectX::XMMATRIX Transform;
    Transform = { DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z) };

    //回転行列から前方向ベクトルを取り出す
    DirectX::XMVECTOR Front = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);

    //注意点から後ろベクトル方向に一定距離離れたカメラ視点を求める
    DirectX::XMFLOAT3 eye;
    eye.x = target.x - front.x * range;
    eye.y = target.y - front.y * range;
    eye.z = target.z - front.z * range;

    //カメラの視点と注意点を設定
    Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}