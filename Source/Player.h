#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Character.h"
#include "Effect.h"

//�v���C���[
class Player : public Character
{
public:
    Player();
    ~Player() override;

    //�X�V����
    void Update(float elapsedTime);
    //�`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* shader);
    //  �f�o�b�O�pGUI�`��
    void DrawDebugGUI();

    //�f�o�b�O�v���~�e�B�u�`��
    void DrawDebugPrimitive();

private:
    //�A�j���[�V����
    enum Animation
    {
        Anim_Attack,
        Anim_Death,
        Anim_Falling,
        Anim_Gethit1,
        Anim_Gethit2,
        Anim_Idle,
        Anim_Jump,
        Anim_Jump_Flip,
        Anim_Landing,
        Anim_Revive,
        Anim_Running,
        Anim_Walking
    };

    //�X�e�[�g
    enum class State
    {
        Idle,
        /*Move,
        Jump,
        Land*/
    };

    Model* model = nullptr;
    Effect* hitEffect = nullptr;
    State state = State::Idle;

    //�X�e�B�b�N���͒l����ړ��x�N�g�����擾
    DirectX::XMFLOAT3 GetMoveVec() const;
    
    float moveSpeed = 5.0f;
    float turnSpeed = DirectX::XMConvertToRadians(720);
    float jumpSpeed = 20.0f;
    
    int jumpCount = 0;
    int jumpLimit = 2;


private:
    
    //�ړ����͏���
    //bool InputMove(float elapsedTime);

    // ���n�X�e�[�g�֑J��
    //void TransitionLandState();
    // ���n�X�e�[�g�X�V����
    //void UpdateLandState(float elapsedTime);

    //�ҋ@�X�e�[�g�֑J��
    void TransitionIdleState();
    //�ҋ@�X�e�[�g�X�V����
    void UpdateIdleState(float elapsedTime);

    //�ړ��X�e�[�g�֑J��
    //void TransitionMoveState();
    //�ړ��X�e�[�g�X�V����
    //void UpdateMoveState(float elapsedTime);

//protected:
    //���n�������ɌĂ΂��
    //void OnLanding() override;

};
