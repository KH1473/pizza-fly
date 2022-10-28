#include "Graphics/Graphics.h"
#include "Graphics/Sprite.h"
#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneManager.h"
#include "Input/Input.h"
#include "SceneLoading.h"
#include "SceneScore.h"
#include "EnemySlime.h"
#include "ScoreDataManager.h"

// 初期化
void SceneScore::Initialize()
{
    // スプライト初期化
    sprite = new Sprite("Data/Sprite/Score.png");

    text = new Sprite("Data/Font/font1.png");
}

// 終了化
void SceneScore::Finalize()
{
    // スプライト終了化
    if (sprite != nullptr)
    {
        delete sprite;
        sprite = nullptr;
    }

    if (text != nullptr)
    {
        delete text;
        text = nullptr;
    }
}

// 更新処理
void SceneScore::Update(float elapsedTime)
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
        float textureWidth = static_cast<float>(sprite->GetTextureWidth());
        float textureHeight = static_cast<float>(sprite->GetTextureHeight());
        // タイトルスプライト描画
        sprite->Render(dc,
            0, 0, screenWidth, screenHeight,
            0, 0, textureWidth, textureHeight,
            0,
            1, 1, 1, 1);


        if (ScoreDataManager::Instance().GetPos() < 20.0f)
        {
            text->textout(dc,
                "SCORE:A",
                200, 0, 96, 96,
                1, 0, 0, 1);
        }
        else if (ScoreDataManager::Instance().GetPos() >= 20.0f)
        {
            text->textout(dc,
                "SCORE:S",
                200, 0, 96, 96,
                1, 0, 0, 1);
        }

    }
    
}