#pragma once

#include "Graph.h"
#include "KamataEngine.h"
#include "Particle.h"
#include "Player.h"
#include "Stage.h"
#include "kMath.h"

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

	// モデル
	KamataEngine::Model* modelParticle_ = nullptr;
	KamataEngine::Model* modelPlayer_ = nullptr;

	// カメラ
	KamataEngine::Camera camera_;

	// 各種オブジェクト
	Particle* particle_ = nullptr;
	Stage* stage_ = nullptr;
	Player* player_ = nullptr;
	Graph* graph_ = nullptr;

	// HP情報
	int hp = 100;
	int maxHp = 100;
};
