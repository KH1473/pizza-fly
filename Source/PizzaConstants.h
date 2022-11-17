#pragma once

/*Dataのファイルの中のEffectファイルの中に宣言してます
  拡張子.efkじゃないと動作確認できてないのでできれば.efkがいいです
  .efkはEffekseer.exe側でエクスポートすると.efkの物が作れます*/

  //ここのみ変更お願いします
  //DEBUG_EFFECT以下の" "の中身を変えると違うものを呼び出せます
  //参考に"Data/Effect/03_Hanmado01/attack.efk"を入れてます

//StageMain.cpp
#define STAGE_MODEL "Data/Model/map1/map1.mdl"  //ステージモデル
#define STAGE_OTHER_ERIA "Data/Model/Cylinder/Cylinder.mdl"  //ステージモデル

//SceneTitle.cpp
#define TITLE_SPRITE "Data/Sprite/Title.png" //タイトル描画
#define PIZZAMODEL "Data/Model/pizzamodel/pizza/pizza.mdl"

//SceneLoading.cpp
#define LOADING_SPRITE "Data/Sprite/LoadingIcon.png" //ローディングアイコン

//SceneSousa.cpp
#define SOUSA_SPRITE "Data/Sprite/sousa.png" //操作描画
#define SOUSA_SSMP "Data/Sprite/MP.png" //マウス操作(緑) もとから見える緑のほう
#define SOUSA_SSRed "Data/Sprite/RED.png" //マウス操作(赤)　触れると反応するほう
#define SOUSA_SSMark "Data/Sprite/mark2.png" //操作説明の回転しているやつ

//SceneGame.cpp
#define GAME_MP "Data/Sprite/MP.png"
#define GAME_Red "Data/Sprite/RED.png"
#define GAME_Back "Data/Sprite/Space.png"
#define KOGE_MODEL "Data/Model/pizzamodel/koge/koge.mdl"

//SceneScore.cpp
#define NAMAYAKE_MODEL "Data/Model/pizzamodel/namayake/namayake.mdl"
#define SCORE_SPRITE "Data/Sprite/Score.png"
#define SCORE_TEXT "Data/Font/font1.png"

//EnemySlime.cpp
//#define PIZZAMODEL "Data/Model/PIZZA/PIZZA.mdl"
//#define PIZZAMODEL "Data/Model/pizzamodel/pizza/pizza.mdl"

//Player.cpp
#define PLAYERMODEL "Data/Model/chr/chr.mdl"
#define DEBUG_EFFECT "Data/Effect/Hit.efk"