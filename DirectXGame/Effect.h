#pragma once
#include "KamataEngine.h"

using namespace KamataEngine;
using namespace MathUtility;

class Effect {
public:
	void Initialize(KamataEngine::Model* model, KamataEngine::Vector3 scale, KamataEngine::Vector3 rotation, KamataEngine::Vector3 position);
	void Update();
	void Draw(KamataEngine::Camera& camera);
	bool GetDeathFlag() { return isFinished_; }

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	ObjectColor objectColor_;
	Vector4 color_;

	static inline const float kDuration = 0.5f;
	bool isFinished_ = false;
	float count_ = 0.0f;

	Vector3 velocity_;      // 爆発の移動ベクトル
	Vector3 originalScale_; // 元のスケール
};
