#pragma once
#include "KamataEngine.h"
#include"kMath.h"

class Player {
public:
	Player();
	~Player();

	// 初期化
	void Initialize(KamataEngine::Model* model, KamataEngine::Vector3 position, KamataEngine::Vector3 velocity);
	// 更新
	void Update();
	// 描画
	void Draw(const KamataEngine::Camera& camera);

	// Player.h
	const KamataEngine::Vector3& GetPosition() const { return worldTransform_.translation_; }


private:
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* playerModel_ = nullptr;
	KamataEngine::Vector3 velocity_;
};
