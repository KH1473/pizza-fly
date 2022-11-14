#pragma once

#include "Player.h"
#include "EnemyManager.h"
#include "EnemySlime.h"
#include "CameraController.h"
#include "Scene.h"
#include "Graphics/Sprite.h"
#include "Audio/Audio.h"

// �Q�[���V�[��
class SceneGame : public Scene
{
public:
	SceneGame() {}
	~SceneGame() override {}

	// ������
	void Initialize() override;

	// �I����
	void Finalize() override;

	// �X�V����
	void Update(float elapsedTime) override;

	// �`�揈��
	void Render() override;

private:
	//Stage* stage = nullptr;
	Player* player = nullptr;
	EnemySlime* slime = nullptr;
	CameraController* cameraController = nullptr;
	//Sprite* gauge = nullptr;
	Sprite* MP = nullptr;
	Sprite* Red = nullptr;

	Sprite* Back = nullptr;

	std::unique_ptr<AudioSource> airhornbgm = nullptr;
	std::unique_ptr<AudioSource> gamebgm = nullptr;

};
