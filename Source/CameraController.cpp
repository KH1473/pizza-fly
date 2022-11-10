#include "CameraController.h"
#include "Camera.h"
#include "Input/Input.h"

//#include <stdlib.h>
#include "SceneManager.h"

//�X�V����
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

    //�J�����̉�]���x
    float speed = rollSpeed * elapsedTime;

    //�X�e�B�b�N�̓��͒l�ɍ��킹��X����Y������]
    angle.x += ay * speed;
    angle.y += ax * speed;

    //X���̃J������]�𐧌�
    if (angle.x >= maxAngleX)
    {
        angle.x = maxAngleX;
    }
    if (angle.x <= minAngleX)
    {
        angle.x = minAngleX;
    }

    //Y���̉�]�l��-3.14~3.14�Ɏ��܂�悤�ɂ���
    if (angle.y < -DirectX::XM_PI)
    {
        angle.y += DirectX::XM_2PI;
    }
    if (angle.y > DirectX::XM_PI)
    {
        angle.y -= DirectX::XM_2PI;
    }
    //�J������]�l����]�s��ɕϊ�
    DirectX::XMMATRIX Transform;
    Transform = { DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z) };

    //��]�s�񂩂�O�����x�N�g�������o��
    DirectX::XMVECTOR Front = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);

    //���ӓ_������x�N�g�������Ɉ�苗�����ꂽ�J�������_�����߂�
    DirectX::XMFLOAT3 eye;
    eye.x = target.x - front.x * range;
    eye.y = target.y - front.y * range;
    eye.z = target.z - front.z * range;

    //�J�����̎��_�ƒ��ӓ_��ݒ�
    Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}