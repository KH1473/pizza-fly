#include "Graphics/Graphics.h"
#include "SceneGame.h"
#include "Camera.h"
#include "EnemyManager.h"
#include "EnemySlime.h"
#include "EffectManager.h"
#include "Input/Input.h"
#include "StageManager.h"
#include "StageMain.h"
#include "PizzaConstants.h"
#include "ScoreDataManager.h"
#include "SceneManager.h"

// ������
void SceneGame::Initialize()
{
	SceneManager::Instance().SetSceneType(SceneType::Main);

	//�X�e�[�W������
	StageManager& stageManager = StageManager::Instance();
	StageMain* stageMain = new StageMain();
	stageManager.Register(stageMain);

	//�v���C���[������
	player = new Player();

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

	MP = new Sprite(GAME_MP);
	Red = new Sprite(GAME_Red);

	Back = new Sprite(GAME_Back);
	
	//�J�n���Đ�
	airhornbgm = Audio::Instance().LoadAudioSource("Data/Audio/�G�A�[�z�[��.wav");
	airhornbgm->Play(false);
	
	//�Q�[�����Đ�
	gamebgm = Audio::Instance().LoadAudioSource("Data/Audio/gamebgm.wav");
	gamebgm->Play(true);
	
	
}

// �I����
void SceneGame::Finalize()
{
	
	//�X�e�[�W�I����
	StageManager::Instance().Clear();

	//�v���C���[�I����
	if (player != nullptr)
	{
		delete player;
		player = nullptr;
	}

	//�J�����R���g���[���[�I����
	if (cameraController != nullptr)
	{
		delete cameraController;
		cameraController = nullptr;
	}

	//�G�l�~�[�I����
	EnemyManager::Instance().Clear();

	// �}�E�X�X�v���C�g�I����
	if (MP != nullptr)
	{
		delete MP;
		MP = nullptr;
	}
	
	if (Red != nullptr)
	{
		delete Red;
		Red = nullptr;
	}
	
	if (Back != nullptr)
	{
		delete Back;
		Back = nullptr;
	}
}

// �X�V����
void SceneGame::Update(float elapsedTime)
{
	//�J�����R���g���[���[�X�V����
	DirectX::XMFLOAT3 target = slime->GetPosition();
	target.y += 0.5f;
	cameraController->SetTarget(target);
	cameraController->Update(elapsedTime);

	//�X�e�[�W�X�V����
	StageManager::Instance().Update(elapsedTime);

	//�v���C���[�X�V����
	player->Update(elapsedTime);

	//�G�l�~�[�X�V����
	EnemyManager::Instance().Update(elapsedTime);

	//�G�t�F�N�g�X�V����
	EffectManager::Instance().Update(elapsedTime);

	// �X�R�A��ʂŕ\�������s�U�̃t�@�C�����̐ݒ�
	if (ScoreDataManager::Instance().GetPos() < 20.0f)
	{
		ScoreDataManager::Instance().SetPizzaModelFilename(NAMAYAKE_MODEL);
	}
	else if (ScoreDataManager::Instance().GetPos() >= 20.0f)
	{
		ScoreDataManager::Instance().SetPizzaModelFilename(KOGE_MODEL);
	}

}

// �`�揈��
void SceneGame::Render()
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
	}

	// 3D���f���`��
	{
		Shader* shader = graphics.GetShader();
		shader->Begin(dc, rc);
		//�X�e�[�W�`��
		StageManager::Instance().Render(dc, shader);

		//�G�l�~�[�`��
		EnemyManager::Instance().Render(dc, shader);

		//�v���C���[�`��
		player->Render(dc, shader);
		shader->End(dc);
		
	}

	//3D�G�t�F�N�g�`��
	{
		EffectManager::Instance().Render(rc.view, rc.projection);
	}

	// 3D�f�o�b�O�`��
	{
		// ���C�������_���`����s
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);

		// �f�o�b�O�����_���`����s
		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
	}

	// 2D�X�v���C�g�`��
	{
		float textureWidth = static_cast<float>(MP->GetTextureWidth());
		float textureHeight = static_cast<float>(MP->GetTextureHeight());

		float RedWidth = static_cast<float>(Red->GetTextureWidth());
		float RedHeight = static_cast<float>(Red->GetTextureHeight());

		//MP�`��
		MP->Render(dc,
			800, 400, textureWidth, textureHeight,
			0, 0, textureWidth, textureHeight,
			0,
			1, 1, 1, 1);

		if (mouse.GetButton() & Mouse::BTN_LEFT)
		{
			if (ax >= 800 && ax <= 950 && ay >= 400 && ay <= 550)
			{
				Red->Render(dc,
					800, 400, RedWidth, RedHeight,
					0, 0, RedWidth, RedHeight,
					0,
					1, 1, 1, 1);
			}
			else if (ax > 950 && ax < 1100 && ay > 400 && ay < 550)
			{
				Red->Render(dc,
					950, 400, RedWidth, RedHeight,
					0, 0, RedWidth, RedHeight,
					0,
					1, 1, 1, 1);
			}
			else if (ax > 950 && ax < 1100 && ay > 550 && ay < 700)
			{
				Red->Render(dc,
					950, 550, RedWidth, RedHeight,
					0, 0, RedWidth, RedHeight,
					0,
					1, 1, 1, 1);
			}
			else if (ax > 800 && ax < 950 && ay > 550 && ay < 700)
			{
				Red->Render(dc,
					800, 550, RedWidth, RedHeight,
					0, 0, RedWidth, RedHeight,
					0,
					1, 1, 1, 1);
			}
		}
	}
}

