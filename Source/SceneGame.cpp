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

// 初期化
void SceneGame::Initialize()
{
	SceneManager::Instance().SetSceneType(SceneType::Main);

	//ステージ初期化
	StageManager& stageManager = StageManager::Instance();
	StageMain* stageMain = new StageMain();
	stageManager.Register(stageMain);

	//プレイヤー初期化
	player = new Player();

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

	//カメラコントローラー初期化
	cameraController = new CameraController();

	//エネミー初期化
	EnemyManager& enemyManager = EnemyManager::Instance();
	
	slime = new EnemySlime();
	slime->SetPosition(DirectX::XMFLOAT3(2.0f, 0, 5));
	enemyManager.Register(slime);

	MP = new Sprite(GAME_MP);
	Red = new Sprite(GAME_Red);

	Back = new Sprite(GAME_Back);
	
	//開始音再生
	airhornbgm = Audio::Instance().LoadAudioSource("Data/Audio/エアーホーン.wav");
	airhornbgm->Play(false);
	
	//ゲーム音再生
	gamebgm = Audio::Instance().LoadAudioSource("Data/Audio/gamebgm.wav");
	gamebgm->Play(true);
	
	
}

// 終了化
void SceneGame::Finalize()
{
	
	//ステージ終了化
	StageManager::Instance().Clear();

	//プレイヤー終了化
	if (player != nullptr)
	{
		delete player;
		player = nullptr;
	}

	//カメラコントローラー終了化
	if (cameraController != nullptr)
	{
		delete cameraController;
		cameraController = nullptr;
	}

	//エネミー終了化
	EnemyManager::Instance().Clear();

	// マウススプライト終了化
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

// 更新処理
void SceneGame::Update(float elapsedTime)
{
	//カメラコントローラー更新処理
	DirectX::XMFLOAT3 target = slime->GetPosition();
	target.y += 0.5f;
	cameraController->SetTarget(target);
	cameraController->Update(elapsedTime);

	//ステージ更新処理
	StageManager::Instance().Update(elapsedTime);

	//プレイヤー更新処理
	player->Update(elapsedTime);

	//エネミー更新処理
	EnemyManager::Instance().Update(elapsedTime);

	//エフェクト更新処理
	EffectManager::Instance().Update(elapsedTime);

	// スコア画面で表示されるピザのファイル名の設定
	if (ScoreDataManager::Instance().GetPos() < 20.0f)
	{
		ScoreDataManager::Instance().SetPizzaModelFilename(NAMAYAKE_MODEL);
	}
	else if (ScoreDataManager::Instance().GetPos() >= 20.0f)
	{
		ScoreDataManager::Instance().SetPizzaModelFilename(KOGE_MODEL);
	}

}

// 描画処理
void SceneGame::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	// 画面クリア＆レンダーターゲット設定
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0～1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// 描画処理
	RenderContext rc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ライト方向（下方向）

	//カメラパラメータ設定
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();

	Mouse& mouse = Input::Instance().GetMouse();
	int ax = mouse.GetPositionX();
	int ay = mouse.GetPositionY();

	// 2Dスプライト描画
	{
		float BscreenWidth = static_cast<float>(graphics.GetScreenWidth());
		float BscreenHeight = static_cast<float>(graphics.GetScreenHeight());
		float BtextureWidth = static_cast<float>(Back->GetTextureWidth());
		float BtextureHeight = static_cast<float>(Back->GetTextureHeight());
		// 背景スプライト描画
		Back->Render(dc,
			0, 0, BscreenWidth, BscreenHeight,
			0, 0, BtextureWidth, BtextureHeight,
			0,
			1, 1, 1, 1);
	}

	// 3Dモデル描画
	{
		Shader* shader = graphics.GetShader();
		shader->Begin(dc, rc);
		//ステージ描画
		StageManager::Instance().Render(dc, shader);

		//エネミー描画
		EnemyManager::Instance().Render(dc, shader);

		//プレイヤー描画
		player->Render(dc, shader);
		shader->End(dc);
		
	}

	//3Dエフェクト描画
	{
		EffectManager::Instance().Render(rc.view, rc.projection);
	}

	// 3Dデバッグ描画
	{
		// ラインレンダラ描画実行
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);

		// デバッグレンダラ描画実行
		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
	}

	// 2Dスプライト描画
	{
		float textureWidth = static_cast<float>(MP->GetTextureWidth());
		float textureHeight = static_cast<float>(MP->GetTextureHeight());

		float RedWidth = static_cast<float>(Red->GetTextureWidth());
		float RedHeight = static_cast<float>(Red->GetTextureHeight());

		//MP描画
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

