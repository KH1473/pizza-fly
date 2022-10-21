#pragma once

#include "Graphics/Sprite.h"
#include "Scene.h"

//説明シーン
class SceneSousa : public Scene
{
public:
    SceneSousa() {}
    ~SceneSousa() override {}

    //初期化
    void Initialize() override;

    //終了化
    void Finalize() override;

    //更新処理
    void Update(float elapsedTime) override;

    //描画処理
    void Render() override;

private:
    Sprite* sprite = nullptr;

    Sprite* SSMP = nullptr;
    Sprite* SSRed = nullptr;

    Sprite* Mark = nullptr;

    float MarkAngle = 0.0f;

};