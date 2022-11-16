#pragma once

#include "Player.h"
#include "EnemyManager.h"
#include "EnemySlime.h"
#include "CameraController.h"
#include "Scene.h"
#include "Graphics/Sprite.h"
#include "Audio/Audio.h"

// ゲームシーン
class SceneGame : public Scene
{
public:
	SceneGame() {}
	~SceneGame() override {}

	// 初期化
	void Initialize() override;

	// 終了化
	void Finalize() override;

	// 更新処理
	void Update(float elapsedTime) override;

	// 描画処理
	void Render() override;

private:
	Player* player = nullptr;
	EnemySlime* slime = nullptr;
	CameraController* cameraController = nullptr;
	
	Sprite* MP = nullptr;
	Sprite* Red = nullptr;
	Sprite* Back = nullptr;
	Sprite* text = nullptr;
	Sprite* timerText = nullptr;

	std::unique_ptr<AudioSource> airhornbgm = nullptr;
	std::unique_ptr<AudioSource> gamebgm = nullptr;

};
