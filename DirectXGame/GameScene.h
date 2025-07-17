#pragma once
#include "KamataEngine.h"
#include "Particle.h"

#include <3d/Camera.h>
#include <3d/Model.h>

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

	// パーティクルモデル
	KamataEngine::Model* modelParticle_ = nullptr;
	// カメラ
	KamataEngine::Camera camera_;
	// パーティクル
	Particle* particle_ = nullptr;
};
