#pragma once
#include "KamataEngine.h"

using namespace KamataEngine;
using namespace MathUtility;

class Effect {
public:
	// 初期化
	void Initialize(KamataEngine::Model* model, KamataEngine::Vector3 scale, KamataEngine::Vector3 rotation, KamataEngine::Vector3 position);

	// 更新
	void Update();

	// 描画
	void Draw(KamataEngine::Camera& camera);

	bool GetDeathFlag() { return isFinished_; }

private:
	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;
	// モデル
	KamataEngine::Model* model_ = nullptr;
	// 色変更オブジェクト
	KamataEngine::ObjectColor objectColor_;
	// 色の数値
	KamataEngine::Vector4 color_;

	//存続時間
	static inline const float kDuration = 0.5f;
	//終了フラグ
	bool isFinished_ = false;
	// カウンター
	float count_ = 0.0f;
};
