#include<imgui.h>
#include "Player.h"
#include "Input/Input.h"
#include "Camera.h"
#include "EnemyManager.h"
#include "Collision.h"
#include"Graphics/Graphics.h"

#include "PizzaConstants.h"

//�R���X�g���N�^
Player::Player()
{
    model = new Model("Data/Model/Jammo/Jammo.mdl");

    //���f�����傫���̂ŃX�P�[�����O
    scale.x = scale.y = scale.z = 0.01f;

    //�q�b�g�G�t�F�N�g�ǂݍ���
    //hitEffect = new Effect("Data/Effect/Hit.efk");
    hitEffect = new Effect(DEBUG_EFFECT);

    //�ҋ@�X�e�[�g�֑J��
    TransitionIdleState();

}

//�f�X�g���N�^
Player::~Player()
{
    delete hitEffect;
    delete model;   
}

//�X�V����
void Player::Update(float elapsedTime)
{
    
    //�X�e�[�g���̏���
    switch (state)
    {
    case State::Idle:UpdateIdleState(elapsedTime);
        break;

    case State::Move:UpdateMoveState(elapsedTime);
        break;
    
    /*case State::Jump:UpdateJumpState(elapsedTime);
        break;*/
    
    case State::Land:UpdateLandState(elapsedTime);
        break;
    }

    //���͏����X�V
    UpdateVelocity(elapsedTime);

    //�I�u�W�F�N�g�s����X�V
    UpdateTransform();

    //���f���A�j���[�V�����X�V����
    model->UpdateAnimation(elapsedTime);

    //���f���s��X�V
    model->UpdateTransform(transform);

}

DirectX::XMFLOAT3 Player::GetMoveVec() const
{
    //���͏����擾
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();

    //�J���������ƃX�e�B�b�N�̓��͒l�ɂ���Đi�s�������v�Z����
    Camera& camera = Camera::Instance();
    const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
    const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

    //�ړ��x�N�g����XYZ���ʂɐ����ɂȂ�悤�ɂ���

    //�J�����E�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
    float cameraRightX = camera.GetRight().x;
    float cameraRightZ = camera.GetRight().z;
    float cameraRightLenght = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
    if (cameraRightLenght > 0.0f)
    {
        //�P�ʃx�N�g����
        cameraRightX / cameraRightLenght;
        cameraRightZ / cameraRightLenght;
    }

    //�J�����O�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
    float cameraFrontX = camera.GetFront().x;
    float cameraFrontZ = camera.GetFront().z;
    float cameraFrontLenght = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);

    if (cameraFrontLenght > 0.0f)
    {
        //�P�ʃx�N�g����
        cameraFrontX / cameraFrontLenght;
        cameraFrontZ / cameraFrontLenght;
    }

    //�X�e�B�b�N�̐������͒l���J�����E�����ɔ��f���A
    //�X�e�B�b�N�̐������͒l���J�����������ɔ��f���A
    //�i�s�x�N�g�����v�Z����
    DirectX::XMFLOAT3 vec;
    vec.x = (ax * cameraRightX) + (ay * cameraFrontX);
    vec.z = (ax * cameraRightZ) + (ay * cameraFrontZ);
    //Y�������ɂ͌v�Z���Ȃ�
    vec.y = 0.0f;

    return vec;
}


//�ړ����͏���
bool Player::InputMove(float elapsedTime)
{
    //�i�s�x�N�g��
    DirectX::XMFLOAT3 moveVec = GetMoveVec();

    //�ړ�����
    Move(moveVec.x, moveVec.z, moveSpeed);

    //���񏈗�
    Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);

    //�i�s�x�N�g�����[���x�N�g���łȂ��ꍇ�͓��͂��ꂽ
    return moveVec.x != 0.0f || moveVec.y != 0.0f || moveVec.z != 0.0f;
    
} 

//�`�揈��
void Player::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);

}

//�f�o�b�O�v���~�e�B�u�`��
void Player::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    //�Փ˔���p�̃f�o�b�O����`��
    //debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

}

//�f�o�b�O�pGUI�`��
void Player::DrawDebugGUI()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Player", nullptr, ImGuiWindowFlags_None))
    {
        //�g�����X�t�H�[��
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            //�ʒu
            ImGui::InputFloat3("Position", &position.x);

            //��]
            DirectX::XMFLOAT3 a;
            a.x = DirectX::XMConvertToDegrees(angle.x);
            a.y = DirectX::XMConvertToDegrees(angle.y);
            a.z = DirectX::XMConvertToDegrees(angle.z);

            ImGui::InputFloat3("Angle", &a.x);
            angle.x = DirectX::XMConvertToRadians(a.x);
            angle.y = DirectX::XMConvertToRadians(a.y);
            angle.z = DirectX::XMConvertToRadians(a.z);

            //�X�P�[��
            ImGui::InputFloat3("Scale", &scale.x);
        }
    }
    ImGui::End();
}


//���n�������ɌĂ΂��
//void Player::OnLanding()
//{
//    jumpCount = 0;
//
//    // �������̑��͂����ȏ�Ȃ璅�n�X�e�[�g��
//    if (velocity.y < gravity * 5.0f)
//    {
//        // ���n�X�e�[�g�֑J��
//        TransitionLandState();
//    }
//
//}

//�ҋ@�X�e�[�g�֑J��
void Player::TransitionIdleState()
{
    state = State::Idle;
    
    //�ҋ@�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Idle, true);

}

//�ҋ@�X�e�[�g�X�V����
void Player::UpdateIdleState(float elapsedTime)
{
    //�ړ����͏���
    if (InputMove(elapsedTime))
    {
        // �ړ��X�e�[�g�֑J��
        TransitionMoveState();
    }

}

//�ړ��X�e�[�g�֑J��
void Player::TransitionMoveState()
{
    state = State::Move;

    //����A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Running, true);
}

//�ړ��X�e�[�g�X�V����
void Player::UpdateMoveState(float elapsedTime)
{
    //�ړ����͏���
    if (!InputMove(elapsedTime))
    {
        TransitionIdleState();
    }

}

//���n�X�e�[�g�֑J��
void Player::TransitionLandState()
{
    state = State::Land;

    //���n�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Landing, false);
}

void Player::UpdateLandState(float elapsedTime)
{
    // ���n�A�j���[�V�����I����
    if (!model->IsPlayAnimation())
    {
        // �ҋ@�X�e�[�g�֑J��
        TransitionIdleState();
    }
}