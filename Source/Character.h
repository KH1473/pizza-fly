#pragma once

#include <DirectXMath.h>

//キャラクター
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

    //行列更新処理
    void UpdateTransform();

    //位置取得
    const DirectX::XMFLOAT3& GetPosition() const { return position; }
    //回転取得
    const DirectX::XMFLOAT3& GetAngle() const { return angle; }
    //スケール取得
    const DirectX::XMFLOAT3& GetScale() const { return scale; }

    //位置設定
    void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

    //回転設定
    void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }

    //スケール設定
    void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

    // 衝撃を与える
    void AddImpulse(const DirectX::XMFLOAT3& impulse);

    // 健康状態を取得
    int GetHealth() const { return health; }
    // 最大健康状態を取得
    int GetMaxHealth() const { return maxHealth; }

    //半径取得
    float GetRadius() const { return radius; }
    //高さ取得
    float GetHeight() const { return height; }

    //地面に接地しているか
    bool IsGround() const { return isGround; }
    
private:

    //垂直移動更新処理
    void UpdateVerticalMove(float elapsedTime);

    // 水平速力更新処理
    void UpdateHorizontalVelocity(float elapsedFrame);

    // 水平移動更新処理
    void UpdateHorizontalMove(float elapsedTime);

protected:

    //移動処理
    void Move(float vx, float vz, float speed);

    //旋回処理
    void Turn(float elapsedTime, float vx, float vz, float speed);

    //遠力処理更新
    void UpdateVelocity(float elapsedTime);

    //着地した時呼ばれる
    virtual void OnLanding() {}

};
