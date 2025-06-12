#pragma once
#include "KamataEngine.h"
#include "Model2.h"

#include <3d/WorldTransform.h>
#include <3d/Model.h>
#include <3d/Camera.h>
#include <base\DirectXCommon.h>


 class GameScene {

public:
	// コンストラクタ
	GameScene();
	// デストラクタ
	~GameScene();

	// 初期化
	void Initialize();
	// 更新
	void Update();
	// 描画
	void Draw();

private:
	KamataEngine::DirectXCommon* dxCommon_ = nullptr;
	KamataEngine::Input* input_ = nullptr;
	KamataEngine::Audio* audio_ = nullptr;
	// カメラ
	KamataEngine::Camera camera_;


};
