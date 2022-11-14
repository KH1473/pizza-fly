#pragma once

//#include "Player.h"
#include "EnemyManager.h"
#include "EnemySlime.h"
#include "CameraController.h"
#include "Scene.h"
#include "Graphics/Sprite.h"
#include "Audio/Audio.h"

// �Q�[���V�[��
class PizzaScore : public Scene
{
public:
	PizzaScore() {}
	
	~PizzaScore() override {}

	// ������
	void Initialize() override;

	// �I����
	void Finalize() override;

	// �X�V����
	void Update(float elapsedTime) override;

	// �`�揈��
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