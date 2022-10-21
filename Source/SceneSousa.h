#pragma once

#include "Graphics/Sprite.h"
#include "Scene.h"

//�����V�[��
class SceneSousa : public Scene
{
public:
    SceneSousa() {}
    ~SceneSousa() override {}

    //������
    void Initialize() override;

    //�I����
    void Finalize() override;

    //�X�V����
    void Update(float elapsedTime) override;

    //�`�揈��
    void Render() override;

private:
    Sprite* sprite = nullptr;

    Sprite* SSMP = nullptr;
    Sprite* SSRed = nullptr;

    Sprite* Mark = nullptr;

    float MarkAngle = 0.0f;

};