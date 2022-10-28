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

// ������
void SceneScore::Initialize()
{
    // �X�v���C�g������
    sprite = new Sprite("Data/Sprite/Score.png");

    text = new Sprite("Data/Font/font1.png");
}

// �I����
void SceneScore::Finalize()
{
    // �X�v���C�g�I����
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

// �X�V����
void SceneScore::Update(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    Mouse& gameMouse = Input::Instance().GetMouse();

    //�����{�^������������^�C�g���V�[���֐؂�ւ�
    const GamePadButton anyButton =
        GamePad::BTN_A
        | GamePad::BTN_B
        | GamePad::BTN_X
        | GamePad::BTN_Y;

    // �Ȃɂ��{�^������������^�C�g���V�[���֐؂�ւ�
    if (gamePad.GetButtonDown() & anyButton || gameMouse.GetButtonDown() & Mouse::BTN_LEFT)
    {
        SceneManager::Instance().ChangeScene(new SceneTitle);
    }

}

// �`�揈��
void SceneScore::Render()
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();
    // ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
    FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f }; // RGBA(0.0�`1.0)
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);
    // 2D�X�v���C�g�`��
    {
        float screenWidth = static_cast<float>(graphics.GetScreenWidth());
        float screenHeight = static_cast<float>(graphics.GetScreenHeight());
        float textureWidth = static_cast<float>(sprite->GetTextureWidth());
        float textureHeight = static_cast<float>(sprite->GetTextureHeight());
        // �^�C�g���X�v���C�g�`��
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