#pragma once

#include "Scene.h"

enum class SceneType : int
{
	Title,
	Tutorial,
	Main,
	Score,
};

//シーンマネージャー
class SceneManager
{
private:
	SceneManager() {}
	~SceneManager() {}

public:
	//唯一のインスタンス取得
	static SceneManager& Instance()
	{
		static SceneManager instance;
		return instance;
	}

	//更新処理
	void Update(float elapsedTime);

	//描画処理
	void Render();

	//シーンクリア
	void Clear();
	
	//シーン切り替え
	void ChangeScene(Scene* scene);

	void SetSceneType(SceneType sceneType) { type = sceneType; }
	SceneType GetSceneType() { return type; }

private:
	Scene* currentScene = nullptr;
	Scene* nextScene = nullptr;

	SceneType type = SceneType::Title;

};
