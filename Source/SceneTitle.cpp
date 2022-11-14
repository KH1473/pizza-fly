#include "Graphics/Graphics.h"
#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneManager.h"
#include "Input/Input.h"
#include "SceneLoading.h"
#include "SceneSousa.h"

#include "PizzaConstants.h"

#include "ScoreDataManager.h"

// ������
void SceneTitle::Initialize()
{
    SceneManager::Instance().SetSceneType(SceneType::Title);

    // �X�v���C�g������
    sprite = new Sprite(TITLE_SPRITE);

    //�^�C�g����BGM�Đ�
    titlebgm = Audio::Instance().LoadAudioSource("Data/Audio/title.wav");
    titlebgm->Play(true);

    // �Q�[����ʂŕ\�������s�U�̃t�@�C�����̐ݒ�
    ScoreDataManager::Instance().SetPizzaModelFilename(NAMAYAKE_MODEL);
}

// �I����
void SceneTitle::Finalize()
{
    // �X�v���C�g�I����
    if (sprite != nullptr)
    {
        delete sprite;
        sprite = nullptr;
    }
   
}

// �X�V����
void SceneTitle::Update(float elapsedTime)
{
    Mouse& gameMouse = Input::Instance().GetMouse();

    // �Ȃɂ��{�^�����������烍�[�f�B���O�V�[��������ŃQ�[���V�[���֐؂�ւ�
    if (gameMouse.GetButtonDown() & Mouse::BTN_LEFT)
    {
        SceneManager::Instance().ChangeScene(new SceneSousa);
    }

}

// �`�揈��
void SceneTitle::Render()
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
    }
}