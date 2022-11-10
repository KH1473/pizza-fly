#include "StageMain.h"

#include "PizzaConstants.h"

//�R���X�g���N�^
StageMain::StageMain()
{
    //�X�e�[�W���f����ǂݍ���
    model = new Model(STAGE_MODEL);

    cylinder = new Model(STAGE_OTHER_ERIA);
    
}

StageMain::~StageMain()
{
    //�X�e�[�W���f���̔j��
    delete model;

    delete cylinder;
}

//�X�V����
void StageMain::Update(float elapsedTime)
{
    //����邱�Ƃ͓��ɂȂ�(from PDF01)
}

//�`�揈��
void StageMain::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    //�V�F�[�_�[�Ƀ��f����`�悵�Ă��炤
    shader->Draw(dc, model);

    shader->Draw(dc, cylinder);
}

// ���C�L���X�g
bool StageMain::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, model, hit);
}