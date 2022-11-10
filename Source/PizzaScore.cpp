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

// 初期化
void PizzaScore::Initialize()
{
	SceneManager::Instance().SetSceneType(SceneType::Score);

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


	// スプライト初期化
	ScoreS = new Sprite(SCORE_SPRITE);
	ScoreA = new Sprite(SCORE_SPRITE);
	Back = new Sprite(SCORE_SPRITE);

}

// 終了化
void PizzaScore::Finalize()
{
	//カメラコントローラー終了化
	if (cameraController != nullptr)
	{
		delete cameraController;
		cameraController = nullptr;
	}

	//エネミー終了化
	EnemyManager::Instance().Clear();


	// スプライト終了化
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

// 更新処理
void PizzaScore::Update(float elapsedTime)
{
	//カメラコントローラー更新処理
	DirectX::XMFLOAT3 target = slime->GetPosition();
	target.y += 0.5f;
	cameraController->SetTarget(target);
	cameraController->Update(elapsedTime);

	//エネミー更新処理
	EnemyManager::Instance().Update(elapsedTime);

	//エフェクト更新処理
	EffectManager::Instance().Update(elapsedTime);

	//シーン操作
	{
		//GamePad& gamePad = Input::Instance().GetGamePad();
		//Mouse& gameMouse = Input::Instance().GetMouse();

		////何かボタンを押したらタイトルシーンへ切り替え
		//const GamePadButton anyButton =
		//	GamePad::BTN_A
		//	| GamePad::BTN_B
		//	| GamePad::BTN_X
		//	| GamePad::BTN_Y;

		//// なにかボタンを押したらタイトルシーンへ切り替え
		//if (gamePad.GetButtonDown() & anyButton || gameMouse.GetButtonDown() & Mouse::BTN_LEFT)
		//{
		//	SceneManager::Instance().ChangeScene(new SceneTitle);
		//}
	}

}

// 描画処理
void PizzaScore::Render()
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

	// 3Dモデル描画
	{
		Shader* shader = graphics.GetShader();
		shader->Begin(dc, rc);
		//エネミー描画
		EnemyManager::Instance().Render(dc, shader);


		shader->End(dc);

	}

	//3Dエフェクト描画
	{
		EffectManager::Instance().Render(rc.view, rc.projection);
	}

	//// 3Dデバッグ描画
	//{
	//	// ラインレンダラ描画実行
	//	graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);

	//	// デバッグレンダラ描画実行
	//	graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
	//}

}
