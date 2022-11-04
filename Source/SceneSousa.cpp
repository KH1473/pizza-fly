#include "Graphics/Graphics.h"
#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneManager.h"
#include "Input/Input.h"
#include "SceneLoading.h"
#include "SceneSousa.h"

#include "PizzaConstants.h"

void SceneSousa::Initialize()
{
    // スプライト初期化
    sprite = new Sprite(SOUSA_SPRITE);

    SSMP = new Sprite(SOUSA_SSMP);
    SSRed = new Sprite(SOUSA_SSRed);
    Mark = new Sprite(SOUSA_SSMark);

}

//終了化
void SceneSousa::Finalize()
{
    // スプライト終了化
    if (sprite != nullptr)
    {
        delete sprite;
        sprite = nullptr;
    }

    //マウススプライト終了化
    if (SSMP != nullptr)
    {
        delete SSMP;
        SSMP = nullptr;
    }
    if (SSRed != nullptr)
    {
        delete SSRed;
        SSRed = nullptr;
    }

    if (Mark != nullptr)
    {
        delete Mark;
        Mark = nullptr;
    }

}

//更新処理
void SceneSousa::Update(float elapsedTime)
{
    constexpr float speed = 180;
    MarkAngle += speed * elapsedTime;

     Mouse& gameMouse = Input::Instance().GetMouse();

     int ax = gameMouse.GetPositionX();
     int ay = gameMouse.GetPositionY();

    // なにかボタンを押したらローディングシーンを挟んでゲームシーンへ切り替え
    if (gameMouse.GetButtonDown() & Mouse::BTN_LEFT)
    {   
        if (ax >= 1125 && ax <= 1270 && ay >= 638 && ay <= 715)
        {
            SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
        }
    }

    /*else if (gameMouse.GetButtonDown() & Mouse::BTN_RIGHT)
    {
        SceneManager::Instance().ChangeScene(new SceneTitle);
    }*/
}

//描画処理
void SceneSousa::Render()
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

        float SStextureWidth = static_cast<float>(SSMP->GetTextureWidth());
        float SStextureHeight = static_cast<float>(SSMP->GetTextureHeight());

        float RedWidth = static_cast<float>(SSRed->GetTextureWidth());
        float RedHeight = static_cast<float>(SSRed->GetTextureHeight());

        //マウス操作描画
        SSMP->Render(dc,
            800, 100, SStextureWidth, SStextureHeight,
            0, 0, SStextureWidth, SStextureHeight,
            0,
            1, 1, 1, 1);

        Mouse& mouse = Input::Instance().GetMouse();
        int ax = mouse.GetPositionX();
        int ay = mouse.GetPositionY();

        if (mouse.GetButton() & Mouse::BTN_LEFT)
        {
            if (ax >= 800 && ax <= 950 && ay >= 100 && ay <= 250)
            {
                SSRed->Render(dc,
                    800, 100, RedWidth, RedHeight,
                    0, 0, RedWidth, RedHeight,
                    0,
                    1, 1, 1, 1);
            }
            else if (ax > 950 && ax < 1100 && ay > 100 && ay < 250)
            {
                SSRed->Render(dc,
                    950, 100, RedWidth, RedHeight,
                    0, 0, RedWidth, RedHeight,
                    0,
                    1, 1, 1, 1);
            }
            else if (ax > 950 && ax < 1100 && ay > 250 && ay < 400)
            {
                SSRed->Render(dc,
                    950, 250, RedWidth, RedHeight,
                    0, 0, RedWidth, RedHeight,
                    0,
                    1, 1, 1, 1);
            }
            else if (ax > 800 && ax < 950 && ay > 250 && ay < 400)
            {
                SSRed->Render(dc,
                    800, 250, RedWidth, RedHeight,
                    0, 0, RedWidth, RedHeight,
                    0,
                    1, 1, 1, 1);
            }
        }

        float MarkWidth = static_cast<float>(Mark->GetTextureWidth());
        float MarkHeight = static_cast<float>(Mark->GetTextureHeight());

        //マーク操作描画
        Mark->Render(dc,
            800, 100, MarkWidth, MarkHeight,
            0, 0, MarkWidth, MarkHeight,
            MarkAngle,
            1, 1, 1, 1);

    }
}
