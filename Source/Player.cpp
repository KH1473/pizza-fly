#include<imgui.h>
#include "Player.h"
#include "Input/Input.h"
#include "Camera.h"
#include "EnemyManager.h"
#include "Collision.h"
#include"Graphics/Graphics.h"

#include "PizzaConstants.h"

//コンストラクタ
Player::Player()
{
    model = new Model("Data/Model/Jammo/Jammo.mdl");

    //モデルが大きいのでスケーリング
    scale.x = scale.y = scale.z = 0.01f;

    //ヒットエフェクト読み込み
    //hitEffect = new Effect("Data/Effect/Hit.efk");
    hitEffect = new Effect(DEBUG_EFFECT);

    //待機ステートへ遷移
    TransitionIdleState();

}

//デストラクタ
Player::~Player()
{
    delete hitEffect;
    delete model;   
}

//更新処理
void Player::Update(float elapsedTime)
{
    
    //ステート毎の処理
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

    //遠力処理更新
    UpdateVelocity(elapsedTime);

    //オブジェクト行列を更新
    UpdateTransform();

    //モデルアニメーション更新処理
    model->UpdateAnimation(elapsedTime);

    //モデル行列更新
    model->UpdateTransform(transform);

}

DirectX::XMFLOAT3 Player::GetMoveVec() const
{
    //入力情報を取得
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();

    //カメラ方向とスティックの入力値によって進行方向を計算する
    Camera& camera = Camera::Instance();
    const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
    const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

    //移動ベクトルはXYZ平面に水平になるようにする

    //カメラ右方向ベクトルをXZ単位ベクトルに変換
    float cameraRightX = camera.GetRight().x;
    float cameraRightZ = camera.GetRight().z;
    float cameraRightLenght = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
    if (cameraRightLenght > 0.0f)
    {
        //単位ベクトル化
        cameraRightX / cameraRightLenght;
        cameraRightZ / cameraRightLenght;
    }

    //カメラ前方向ベクトルをXZ単位ベクトルに変換
    float cameraFrontX = camera.GetFront().x;
    float cameraFrontZ = camera.GetFront().z;
    float cameraFrontLenght = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);

    if (cameraFrontLenght > 0.0f)
    {
        //単位ベクトル化
        cameraFrontX / cameraFrontLenght;
        cameraFrontZ / cameraFrontLenght;
    }

    //スティックの水平入力値をカメラ右方向に反映し、
    //スティックの垂直入力値をカメラ左方向に反映し、
    //進行ベクトルを計算する
    DirectX::XMFLOAT3 vec;
    vec.x = (ax * cameraRightX) + (ay * cameraFrontX);
    vec.z = (ax * cameraRightZ) + (ay * cameraFrontZ);
    //Y軸方向には計算しない
    vec.y = 0.0f;

    return vec;
}


//移動入力処理
bool Player::InputMove(float elapsedTime)
{
    //進行ベクトル
    DirectX::XMFLOAT3 moveVec = GetMoveVec();

    //移動処理
    Move(moveVec.x, moveVec.z, moveSpeed);

    //旋回処理
    Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);

    //進行ベクトルがゼロベクトルでない場合は入力された
    return moveVec.x != 0.0f || moveVec.y != 0.0f || moveVec.z != 0.0f;
    
} 

//描画処理
void Player::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);

}

//デバッグプリミティブ描画
void Player::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    //衝突判定用のデバッグ球を描画
    //debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

}

//デバッグ用GUI描画
void Player::DrawDebugGUI()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Player", nullptr, ImGuiWindowFlags_None))
    {
        //トランスフォーム
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            //位置
            ImGui::InputFloat3("Position", &position.x);

            //回転
            DirectX::XMFLOAT3 a;
            a.x = DirectX::XMConvertToDegrees(angle.x);
            a.y = DirectX::XMConvertToDegrees(angle.y);
            a.z = DirectX::XMConvertToDegrees(angle.z);

            ImGui::InputFloat3("Angle", &a.x);
            angle.x = DirectX::XMConvertToRadians(a.x);
            angle.y = DirectX::XMConvertToRadians(a.y);
            angle.z = DirectX::XMConvertToRadians(a.z);

            //スケール
            ImGui::InputFloat3("Scale", &scale.x);
        }
    }
    ImGui::End();
}


//着地した時に呼ばれる
//void Player::OnLanding()
//{
//    jumpCount = 0;
//
//    // 下方向の速力が一定以上なら着地ステートへ
//    if (velocity.y < gravity * 5.0f)
//    {
//        // 着地ステートへ遷移
//        TransitionLandState();
//    }
//
//}

//待機ステートへ遷移
void Player::TransitionIdleState()
{
    state = State::Idle;
    
    //待機アニメーション再生
    model->PlayAnimation(Anim_Idle, true);

}

//待機ステート更新処理
void Player::UpdateIdleState(float elapsedTime)
{
    //移動入力処理
    if (InputMove(elapsedTime))
    {
        // 移動ステートへ遷移
        TransitionMoveState();
    }

}

//移動ステートへ遷移
void Player::TransitionMoveState()
{
    state = State::Move;

    //走りアニメーション再生
    model->PlayAnimation(Anim_Running, true);
}

//移動ステート更新処理
void Player::UpdateMoveState(float elapsedTime)
{
    //移動入力処理
    if (!InputMove(elapsedTime))
    {
        TransitionIdleState();
    }

}

//着地ステートへ遷移
void Player::TransitionLandState()
{
    state = State::Land;

    //着地アニメーション再生
    model->PlayAnimation(Anim_Landing, false);
}

void Player::UpdateLandState(float elapsedTime)
{
    // 着地アニメーション終了後
    if (!model->IsPlayAnimation())
    {
        // 待機ステートへ遷移
        TransitionIdleState();
    }
}