#include "Graphics/Graphics.h"
#include "SceneTitle.h"
#include "Graphics/Sprite.h"
#include "EnemySlime.h"
#include "Input/Input.h"
#include "Camera.h"
//#include "SceneGame.h"
//#include "SceneLoading.h"
#include "SceneManager.h"
#include "SceneScore.h"
#include "ScoreDataManager.h"

#include "PizzaConstants.h"

// 初期化
void SceneScore::Initialize()
{
    //カメラ初期設定
    Graphics& graphics = Graphics::Instance();
    Camera& camera = Camera::Instance();
    camera.SetLookAt(
        DirectX::XMFLOAT3(0, 10, -10),
        DirectX::XMFLOAT3(0, 0, 0),
        DirectX::XMFLOAT3(0, 1, 0)
    );
    camera.SetPerspectiveFov(
        DirectX::XMConvertToRadians(45),
        graphics.GetScreenWidth() / graphics.GetScreenHeight(),
        0.1f,
        1000.0f
    );

    // スプライト初期化
    Score = new Sprite(SCORE_SPRITE);

    if (ScoreDataManager::Instance().GetPizzaModelFilename() == KOGE_MODEL)
    {
        PizzaRank = new Sprite("Data/Sprite/PizzaRank/prank1.png");
    }
    else if (ScoreDataManager::Instance().GetPizzaModelFilename() == NAMAYAKE_MODEL)
    {
        PizzaRank = new Sprite("Data/Sprite/PizzaRank/prank2.png");
    }
    else if (ScoreDataManager::Instance().GetPizzaModelFilename() == PIZZAMODEL)
    {
        PizzaRank = new Sprite("Data/Sprite/PizzaRank/prank3.png");
    }
}

// 終了化
void SceneScore::Finalize()
{
    // スプライト終了化
    if (Score != nullptr)
    {
        delete Score;
        Score = nullptr;
    }

    if (PizzaRank != nullptr)
    {
        delete PizzaRank;
        PizzaRank = nullptr;
    }
}

// 更新処理
void SceneScore::Update(float elapsedTime)
{

    //シーン操作
    {
        GamePad& gamePad = Input::Instance().GetGamePad();
        Mouse& gameMouse = Input::Instance().GetMouse();

        //何かボタンを押したらタイトルシーンへ切り替え
        const GamePadButton anyButton =
            GamePad::BTN_A
            | GamePad::BTN_B
            | GamePad::BTN_X
            | GamePad::BTN_Y;

        // なにかボタンを押したらタイトルシーンへ切り替え
        if (gamePad.GetButtonDown() & anyButton || gameMouse.GetButtonDown() & Mouse::BTN_LEFT)
        {
            SceneManager::Instance().ChangeScene(new SceneTitle);
        }
    }
}

// 描画処理
void SceneScore::Render()
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();
    // 画面クリア＆レンダーターゲット設定
    FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f }; // RGBA(0.0～1.0)
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);
    // 2Dスプライト描画
    {
        float screenWidth = static_cast<float>(graphics.GetScreenWidth());
        float screenHeight = static_cast<float>(graphics.GetScreenHeight());
        float textureWidth = static_cast<float>(Score->GetTextureWidth());
        float textureHeight = static_cast<float>(Score->GetTextureHeight());


            Score->Render(dc,
                0, 0, screenWidth, screenHeight,
                0, 0, textureWidth, textureHeight,
                0,
                1, 1, 1, 1);


          float PizzaRankWidth = static_cast<float>(PizzaRank->GetTextureWidth());
          float PizzaRankHeight = static_cast<float>(PizzaRank->GetTextureHeight());

            PizzaRank->Render(dc,
                0, 0, PizzaRankWidth, PizzaRankHeight,
                0, 0, PizzaRankWidth, PizzaRankHeight,
                0,
                1, 1, 1, 1);
    }

}