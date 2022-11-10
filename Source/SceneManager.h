#pragma once

#include "Scene.h"

enum class SceneType : int
{
	Title,
	Tutorial,
	Main,
	Score,
};

//�V�[���}�l�[�W���[
class SceneManager
{
private:
	SceneManager() {}
	~SceneManager() {}

public:
	//�B��̃C���X�^���X�擾
	static SceneManager& Instance()
	{
		static SceneManager instance;
		return instance;
	}

	//�X�V����
	void Update(float elapsedTime);

	//�`�揈��
	void Render();

	//�V�[���N���A
	void Clear();
	
	//�V�[���؂�ւ�
	void ChangeScene(Scene* scene);

	void SetSceneType(SceneType sceneType) { type = sceneType; }
	SceneType GetSceneType() { return type; }

private:
	Scene* currentScene = nullptr;
	Scene* nextScene = nullptr;

	SceneType type = SceneType::Title;

};
