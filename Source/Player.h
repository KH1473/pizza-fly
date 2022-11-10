#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Character.h"
#include "Effect.h"

//プレイヤー
class Player : public Character
{
public:
    Player();
    ~Player() override;

    //更新処理
    void Update(float elapsedTime);
    //描画処理
    void Render(ID3D11DeviceContext* dc, Shader* shader);
    //  デバッグ用GUI描画
    void DrawDebugGUI();

    //デバッグプリミティブ描画
    void DrawDebugPrimitive();

private:
    //アニメーション
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

    //ステート
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

    //スティック入力値から移動ベクトルを取得
    DirectX::XMFLOAT3 GetMoveVec() const;
    
    float moveSpeed = 5.0f;
    float turnSpeed = DirectX::XMConvertToRadians(720);
    float jumpSpeed = 20.0f;
    
    int jumpCount = 0;
    int jumpLimit = 2;


private:
    
    //移動入力処理
    //bool InputMove(float elapsedTime);

    // 着地ステートへ遷移
    //void TransitionLandState();
    // 着地ステート更新処理
    //void UpdateLandState(float elapsedTime);

    //待機ステートへ遷移
    void TransitionIdleState();
    //待機ステート更新処理
    void UpdateIdleState(float elapsedTime);

    //移動ステートへ遷移
    //void TransitionMoveState();
    //移動ステート更新処理
    //void UpdateMoveState(float elapsedTime);

//protected:
    //着地した時に呼ばれる
    //void OnLanding() override;

};
