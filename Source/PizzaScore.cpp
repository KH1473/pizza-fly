#include "Graphics/Graphics.h"
#include "Graphics/Sprite.h"
#include "Camera.h"

#include "SceneManager.h"
#include "SceneGame.h"
#include "PizzaScore.h"
#include "SceneTitle.h"

#include "EnemyManager.h"
#include "EnemySlime.h"
#include "EffectManager.h"
#include "Input/Input.h"

#include "ScoreDataManager.h"

#include "PizzaConstants.h"

// ������
void PizzaScore::Initialize()
{
	SceneManager::Instance().SetSceneType(SceneType::Score);

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

	//�J�����R���g���[���[������
	cameraController = new CameraController();

	//�G�l�~�[������
	EnemyManager& enemyManager = EnemyManager::Instance();

	slime = new EnemySlime();
	slime->SetPosition(DirectX::XMFLOAT3(2.0f, 0, 5));
	enemyManager.Register(slime);


	// �X�v���C�g������
	ScoreS = new Sprite(SCORE_SPRITE);
	ScoreA = new Sprite(SCORE_SPRITE);
	Back = new Sprite(SCORE_SPRITE);

}

// �I����
void PizzaScore::Finalize()
{
	//�J�����R���g���[���[�I����
	if (cameraController != nullptr)
	{
		delete cameraController;
		cameraController = nullptr;
	}

	//�G�l�~�[�I����
	EnemyManager::Instance().Clear();


	// �X�v���C�g�I����
	if (ScoreS != nullptr)
	{
		delete ScoreS;
		ScoreS = nullptr;
	}

	if (ScoreA != nullptr)
	{
		delete ScoreA;
		ScoreA = nullptr;
	}

	if (Back != nullptr)
	{
		delete Back;
		Back = nullptr;
	}
}

// �X�V����
void PizzaScore::Update(float elapsedTime)
{
	//�J�����R���g���[���[�X�V����
	DirectX::XMFLOAT3 target = slime->GetPosition();
	target.y += 0.5f;
	cameraController->SetTarget(target);
	cameraController->Update(elapsedTime);

	//�G�l�~�[�X�V����
	EnemyManager::Instance().Update(elapsedTime);

	//�G�t�F�N�g�X�V����
	EffectManager::Instance().Update(elapsedTime);

	//�V�[������
	{
		//GamePad& gamePad = Input::Instance().GetGamePad();
		//Mouse& gameMouse = Input::Instance().GetMouse();

		////�����{�^������������^�C�g���V�[���֐؂�ւ�
		//const GamePadButton anyButton =
		//	GamePad::BTN_A
		//	| GamePad::BTN_B
		//	| GamePad::BTN_X
		//	| GamePad::BTN_Y;

		//// �Ȃɂ��{�^������������^�C�g���V�[���֐؂�ւ�
		//if (gamePad.GetButtonDown() & anyButton || gameMouse.GetButtonDown() & Mouse::BTN_LEFT)
		//{
		//	SceneManager::Instance().ChangeScene(new SceneTitle);
		//}
	}

}

// �`�揈��
void PizzaScore::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	// ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0�`1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// �`�揈��
	RenderContext rc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ���C�g�����i�������j

	//�J�����p�����[�^�ݒ�
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();

	Mouse& mouse = Input::Instance().GetMouse();
	int ax = mouse.GetPositionX();
	int ay = mouse.GetPositionY();

	// 2D�X�v���C�g�`��
	{
		float BscreenWidth = static_cast<float>(graphics.GetScreenWidth());
		float BscreenHeight = static_cast<float>(graphics.GetScreenHeight());
		float BtextureWidth = static_cast<float>(Back->GetTextureWidth());
		float BtextureHeight = static_cast<float>(Back->GetTextureHeight());
		// �w�i�X�v���C�g�`��
		Back->Render(dc,
			0, 0, BscreenWidth, BscreenHeight,
			0, 0, BtextureWidth, BtextureHeight,
			0,
			1, 1, 1, 1);

		/*float screenWidth = static_cast<float>(graphics.GetScreenWidth());
		float screenHeight = static_cast<float>(graphics.GetScreenHeight());
		float textureWidth = static_cast<float>(ScoreS->GetTextureWidth());
		float textureHeight = static_cast<float>(ScoreS->GetTextureHeight());


		if (ScoreDataManager::Instance().GetPos() < 20.0f)
		{
			ScoreA->Render(dc,
				0, 0, screenWidth, screenHeight,
				0, 0, textureWidth, textureHeight,
				0,
				1, 1, 1, 1);
		}
		else if (ScoreDataManager::Instance().GetPos() >= 20.0f)
		{
			ScoreS->Render(dc,
				0, 0, screenWidth, screenHeight,
				0, 0, textureWidth, textureHeight,
				0,
				1, 1, 1, 1);
		}*/
	}

	// 3D���f���`��
	{
		Shader* shader = graphics.GetShader();
		shader->Begin(dc, rc);
		//�G�l�~�[�`��
		EnemyManager::Instance().Render(dc, shader);


		shader->End(dc);

	}

	//3D�G�t�F�N�g�`��
	{
		EffectManager::Instance().Render(rc.view, rc.projection);
	}

	//// 3D�f�o�b�O�`��
	//{
	//	// ���C�������_���`����s
	//	graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);

	//	// �f�o�b�O�����_���`����s
	//	graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
	//}

}
