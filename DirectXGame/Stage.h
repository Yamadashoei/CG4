#pragma once
#include "KamataEngine.h"
#include <3d/Camera.h>
#include <3d/Model.h>
#include <3d/WorldTransform.h>
#include <base/DirectXCommon.h>

class Stage {
public:
	// 統一シグネチャ
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& center, float startRadius, float endRadius, float durationSec);

	void Update();                                      // 固定Δtで進める（必要なら後で実Δt化）
	void Draw();                                        // カメラは内部参照を使用
	void SetPosition(const KamataEngine::Vector3& pos); // 任意（center_も更新）

	// 参照用
	float Radius() const { return radius_; }
	const KamataEngine::Vector3& Center() const { return center_; }

private:
	float Ease(float x) const { return x; } // 線形。必要ならイージングに差替

private:
	KamataEngine::DirectXCommon* dxCommon_ = nullptr;
	KamataEngine::Input* input_ = nullptr;
	KamataEngine::Audio* audio_ = nullptr;

	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* cameraRef_ = nullptr;

	KamataEngine::WorldTransform worldTransform_;
	uint32_t textureHandle_ = 0;

	// 縮小プロファイル
	KamataEngine::Vector3 center_{0, 0, 0};
	float startR_ = 10.0f;
	float endR_ = 3.0f;
	float duration_ = 60.0f; // 秒

	float t_ = 0.0f; // 0→1
	float radius_ = 10.0f;

	static constexpr float kFixedDt_ = 1.0f / 60.0f;
};
