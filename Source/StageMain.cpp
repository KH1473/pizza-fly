#include "StageMain.h"

#include "PizzaConstants.h"

//コンストラクタ
StageMain::StageMain()
{
    //ステージモデルを読み込む
    model = new Model(STAGE_MODEL);

    cylinder = new Model(STAGE_OTHER_ERIA);
    
}

StageMain::~StageMain()
{
    //ステージモデルの破棄
    delete model;

    delete cylinder;
}

//更新処理
void StageMain::Update(float elapsedTime)
{
    //今やることは特になし(from PDF01)
}

//描画処理
void StageMain::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    //シェーダーにモデルを描画してもらう
    shader->Draw(dc, model);

    shader->Draw(dc, cylinder);
}

// レイキャスト
bool StageMain::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, model, hit);
}