#include "Graphics/Graphics.h"
#include "SceneGame.h"
#include "Camera.h"
#include "EnemyManager.h"
#include "EnemySlime.h"
#include "EffectManager.h"
#include "Input/Input.h"
#include "StageManager.h"
#include "StageMain.h"
#include "StageMoveFloor.h"

// 初期化
void SceneGame::Initialize()
{
	//ステージ初期化
	//stage = new Stage();
	StageManager& stageManager = StageManager::Instance();
	StageMain* stageMain = new StageMain();
	stageManager.Register(stageMain);

	/*StageMoveFloor* stageMoveFloor = new StageMoveFloor();
	stageMoveFloor->SetStartPoint(DirectX::XMFLOAT3(0, 1, 3));
	stageMoveFloor->SetGoalPoint(DirectX::XMFLOAT3(10, 2, 3));
	stageMoveFloor->SetTorque(DirectX::XMFLOAT3(0, 1.0f, 0));
	stageManager.Register(stageMoveFloor);*/

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
	//EnemySlime* slime = new EnemySlime();
	//slime->SetPosition(DirectX::XMFLOAT3(0, 0, 5));
	//enemyManager.Register(slime);
	/*for (int i = 0; i < 1; ++i)
	{	
		EnemySlime* slime = new EnemySlime();
		slime->SetPosition(DirectX::XMFLOAT3(i * 2.0f, 0, 5));
		enemyManager.Register(slime);
	}*/
	slime = new EnemySlime();
	slime->SetPosition(DirectX::XMFLOAT3(2.0f, 0, 5));
	enemyManager.Register(slime);
	
	
	//ゲージスプライト
	//gauge = new Sprite();

	MP = new Sprite("Data/Sprite/MP.png");
	Red = new Sprite("Data/Sprite/RED.png");

	Back = new Sprite("Data/Sprite/Space.png");

}

// 終了化
void SceneGame::Finalize()
{
	// ゲージスプライト終了化
	/*if (gauge != nullptr)
	{
		delete gauge;
		gauge = nullptr;
	}*/

	//ステージ終了化
	/*if (stage != nullptr)
	{
		delete stage;
		stage = nullptr;
	}*/
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
	//DirectX::XMFLOAT3 target = player->GetPosition();
	DirectX::XMFLOAT3 target = slime->GetPosition();
	target.y += 0.5f;
	cameraController->SetTarget(target);
	cameraController->Update(elapsedTime);

	//ステージ更新処理
	//stage->Update(elapsedTime);
	StageManager::Instance().Update(elapsedTime);

	//プレイヤー更新処理
	player->Update(elapsedTime);

	//エネミー更新処理
	EnemyManager::Instance().Update(elapsedTime);

	//エフェクト更新処理
	EffectManager::Instance().Update(elapsedTime);

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

	// ビュー行列
	/* {
		DirectX::XMFLOAT3 eye = { 0, 10, -10 };	// カメラの視点（位置）
		DirectX::XMFLOAT3 focus = { 0, 0, 0 };	// カメラの注視点（ターゲット）
		DirectX::XMFLOAT3 up = { 0, 1, 0 };		// カメラの上方向

		DirectX::XMVECTOR Eye = DirectX::XMLoadFloat3(&eye);
		DirectX::XMVECTOR Focus = DirectX::XMLoadFloat3(&focus);
		DirectX::XMVECTOR Up = DirectX::XMLoadFloat3(&up);
		DirectX::XMMATRIX View = DirectX::XMMatrixLookAtLH(Eye, Focus, Up);
		DirectX::XMStoreFloat4x4(&rc.view, View);
	}*/
	// プロジェクション行列
	/*{
		float fovY = DirectX::XMConvertToRadians(45);	// 視野角
		float aspectRatio = graphics.GetScreenWidth() / graphics.GetScreenHeight();	// 画面縦横比率
		float nearZ = 0.1f;	// カメラが映し出すの最近距離
		float farZ = 1000.0f;	// カメラが映し出すの最遠距離
		DirectX::XMMATRIX Projection = DirectX::XMMatrixPerspectiveFovLH(fovY, aspectRatio, nearZ, farZ);
		DirectX::XMStoreFloat4x4(&rc.projection, Projection);
	}*/


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
		//stage->Render(dc, shader);
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
		//プレイヤーデバッグプリミティブ描画
		//player->DrawDebugPrimitive();

		// エネミーデバッグプリミティブ描画
		//EnemyManager::Instance().DrawDebugPrimitive();

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

		//RenderEnemyGauge(dc, rc.view, rc.projection);
		MP->Render(dc,
			800, 400, textureWidth, textureHeight,
			0, 0, textureWidth, textureHeight,
			0,
			1, 1, 1, 1);

		if (mouse.GetButton() & Mouse::BTN_LEFT) {
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

	// 2DデバッグGUI描画
	{
		//プレイヤーデバッグ描画
		//player->DrawDebugGUI();
	}
}

// エネミーHPゲージ描画
/*void SceneGame::RenderEnemyGauge(
	ID3D11DeviceContext* dc,
	const DirectX::XMFLOAT4X4& view,
	const DirectX::XMFLOAT4X4& projection)
{
	// ビューポート
	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	dc->RSGetViewports(&numViewports, &viewport);
	
	// 変換行列
	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();
	
	// 全ての敵の頭上にHPゲージを表示
	EnemyManager& enemyManager = EnemyManager::Instance();
	int enemyCount = enemyManager.GetEnemyCount();
	for (int i = 0; i < enemyCount; ++i)
	{
		Enemy* enemy = enemyManager.GetEnemy(i);
		
		// エネミーの頭上のワールド座標
		DirectX::XMFLOAT3 worldPosition = enemy->GetPosition();
		worldPosition.y += enemy->GetHeight();
		DirectX::XMVECTOR WorldPosition = DirectX::XMLoadFloat3(&worldPosition);
		
		// ワールド座標からスクリーン座標へ変換
		DirectX::XMVECTOR ScreenPosition = DirectX::XMVector3Project(
			WorldPosition,
			viewport.TopLeftX,
			viewport.TopLeftY,
			viewport.Width,
			viewport.Height,
			viewport.MinDepth,
			viewport.MaxDepth,
			Projection,
			View,
			World
		);
		
		// スクリーン座標
		DirectX::XMFLOAT2 screenPosition;
		DirectX::XMStoreFloat2(&screenPosition, ScreenPosition);
		
		// ゲージの長さ
		const float gaugeWidth = 30.0f;
		const float gaugeHeight = 5.0f;
		float healthRate = enemy->GetHealth() / static_cast<float>(enemy->GetMaxHealth());
		
		// ゲージ描画
		gauge->Render(dc,
			screenPosition.x - gaugeWidth * 0.5f,
			screenPosition.y - gaugeHeight,
			gaugeWidth * healthRate,
			gaugeHeight,
			0, 0,
			static_cast<float>(gauge->GetTextureWidth()),
			static_cast<float>(gauge->GetTextureHeight()),
			0.0f,
			1.0f, 0.0f, 0.0f, 1.0f
		);
	}

	// エネミー配置処理
	Mouse& mouse = Input::Instance().GetMouse();
	if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
	{
		// マウスカーソル座標を取得
		DirectX::XMFLOAT3 screenPosition;
		screenPosition.x = static_cast<float>(mouse.GetPositionX());
		screenPosition.y = static_cast<float>(mouse.GetPositionY());
		DirectX::XMVECTOR ScreenPosition, WorldPosition;
		
		// レイの始点を算出
		screenPosition.z = 0.0f;
		ScreenPosition = DirectX::XMLoadFloat3(&screenPosition);
		WorldPosition = DirectX::XMVector3Unproject(
			ScreenPosition,
			viewport.TopLeftX,
			viewport.TopLeftY,
			viewport.Width,
			viewport.Height,
			viewport.MinDepth,
			viewport.MaxDepth,
			Projection,
			View,
			World
		);
		DirectX::XMFLOAT3 rayStart;
		DirectX::XMStoreFloat3(&rayStart, WorldPosition);
		
		// レイの終点を算出
		screenPosition.z = 1.0f;
		ScreenPosition = DirectX::XMLoadFloat3(&screenPosition);
		WorldPosition = DirectX::XMVector3Unproject(
			ScreenPosition,
			viewport.TopLeftX,
			viewport.TopLeftY,
			viewport.Width,
			viewport.Height,
			viewport.MinDepth,
			viewport.MaxDepth,
			Projection,
			View,
			World
		);
		DirectX::XMFLOAT3 rayEnd;
		DirectX::XMStoreFloat3(&rayEnd, WorldPosition);
		// レイキャスト
		HitResult hit;
		//if (stage->RayCast(rayStart, rayEnd, hit))
		if (StageManager::Instance().RayCast(rayStart, rayEnd, hit))
		{
			// 敵を配置
			EnemySlime* slime = new EnemySlime();
			slime->SetPosition(hit.position);
			EnemyManager::Instance().Register(slime);
		}
	}

}*/
