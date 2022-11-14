#pragma once

//#include "Player.h"
#include "EnemyManager.h"
#include "EnemySlime.h"
#include "CameraController.h"
#include "Scene.h"
#include "Graphics/Sprite.h"
#include "Audio/Audio.h"

// ゲームシーン
class PizzaScore : public Scene
{
public:
	PizzaScore() {}
	
	~PizzaScore() override {}

	// 初期化
	void Initialize() override;

	// 終了化
	void Finalize() override;

	// 更新処理
	void Update(float elapsedTime) override;

	// 描画処理
	void Render() override;

private:
	EnemySlime* slime = nullptr;
	CameraController* cameraController = nullptr;
	
	Sprite* ScoreS = nullptr;
	Sprite* ScoreA = nullptr;
	Sprite* Back = nullptr;

	std::unique_ptr<AudioSource> kogebgm = nullptr;
	std::unique_ptr<AudioSource> clearbgm = nullptr;

};