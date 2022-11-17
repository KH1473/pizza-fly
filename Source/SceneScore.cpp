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

// ������
void SceneScore::Initialize()
{
    //�J���������ݒ�
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

    // �X�v���C�g������
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

// �I����
void SceneScore::Finalize()
{
    // �X�v���C�g�I����
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

// �X�V����
void SceneScore::Update(float elapsedTime)
{

    //�V�[������
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