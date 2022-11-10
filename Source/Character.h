#pragma once

#include <DirectXMath.h>

//�L�����N�^�[
class Character
{
protected:
    DirectX::XMFLOAT3   position  = { 2.0f, 0, 5.0f };
    DirectX::XMFLOAT3   angle     = { 0, 0, 0 };
    DirectX::XMFLOAT3   scale     = { 1, 1, 1 };
    DirectX::XMFLOAT3   velocity  = { 0, 0, 0 };
    DirectX::XMFLOAT4X4 transform = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };

    int health            = 5;
    int maxHealth         = 5;

    float radius          = 0.5f;
    float gravity         = -1.0f;
    
    float height          = 1.45f;
    
    float invincibleTimer = 1.0f;
    float friction        = 0.5f;
    float acceleration    = 1.0f;
    float maxMoveSpeed    = 5.0f;
    float moveVecX        = 0.0f;
    float moveVecZ        = 0.0f;
    float airControl      = 0.3f;
    float stepOffset      = 1.0f;
    float slopeRate       = 1.0f;

    bool isGround = false;

public:
    Character() {}
    virtual ~Character() {}

    //�s��X�V����
    void UpdateTransform();

    //�ʒu�擾
    const DirectX::XMFLOAT3& GetPosition() const { return position; }
    //��]�擾
    const DirectX::XMFLOAT3& GetAngle() const { return angle; }
    //�X�P�[���擾
    const DirectX::XMFLOAT3& GetScale() const { return scale; }

    //�ʒu�ݒ�
    void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

    //��]�ݒ�
    void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }

    //�X�P�[���ݒ�
    void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

    // �Ռ���^����
    void AddImpulse(const DirectX::XMFLOAT3& impulse);

    // ���N��Ԃ��擾
    int GetHealth() const { return health; }
    // �ő匒�N��Ԃ��擾
    int GetMaxHealth() const { return maxHealth; }

    //���a�擾
    float GetRadius() const { return radius; }
    //�����擾
    float GetHeight() const { return height; }

    //�n�ʂɐڒn���Ă��邩
    bool IsGround() const { return isGround; }
    
private:

    //�����ړ��X�V����
    void UpdateVerticalMove(float elapsedTime);

    // �������͍X�V����
    void UpdateHorizontalVelocity(float elapsedFrame);

    // �����ړ��X�V����
    void UpdateHorizontalMove(float elapsedTime);

protected:

    //�ړ�����
    void Move(float vx, float vz, float speed);

    //���񏈗�
    void Turn(float elapsedTime, float vx, float vz, float speed);

    //���͏����X�V
    void UpdateVelocity(float elapsedTime);

    //���n�������Ă΂��
    virtual void OnLanding() {}

};
