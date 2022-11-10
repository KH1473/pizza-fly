#pragma once

#include "Graphics/Sprite.h"
#include "Scene.h"

//�Q�[���X�R�A�V�[��
class SceneScore : public Scene
{
public:
    SceneScore() {}
    ~SceneScore() override {}

    //������
    void Initialize() override;

    //�I����
    void Finalize() override;

    //�X�V����
    void Update(float elapsedTime) override;

    //�`�揈��
    void Render() override;


private:
   
    Sprite* ScoreS = nullptr;
    Sprite* ScoreA = nullptr;

};
