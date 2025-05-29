#pragma once
#include "Effect.h"
#include "KamataEngine.h"
#include <list>
using namespace KamataEngine;

class GameScene {
public:
	// デストラクタ
	~GameScene();

	// 初期化
	void Initialize();
	// 更新
	void Update();
	// 描画
	void Draw();

	//エフェクトの発生関数
	void EffectBorn(KamataEngine::Vector3 position);


private:
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// エフェクトの3Dモデルデータ
	Model* modelEffect_ = nullptr;
	// エフェクトの複数化
	std::list<Effect*> effects_;
	// カメラ
	Camera camera_;
};
