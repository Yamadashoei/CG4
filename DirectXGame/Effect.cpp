#include "Effect.h"
#include <cassert>
#include <algorithm>

using namespace KamataEngine;
using namespace MathUtility;

void Effect::Initialize(Model* model, Vector3 scale, Vector3 rotation,Vector3 position) {
	assert(model);
	model_ = model;

	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.scale_ = scale;
	worldTransform_.rotation_ = rotation;
	worldTransform_.translation_ = position;
	//worldTransform_.translation_ = {0.0f, 0.0f, 0.0f}; // 原点固定（安全のため明示）

	// 色の設定
	objectColor_.Initialize();
	color_ = {1, 1, 1, 1};
}

void Effect::Update() {
	// 終了ならリターン
	if (isFinished_) {
		return;
	}
	// カウンターを1フレーム分の秒数進める
	count_ += 1.0f / 60.0f;
	if (count_ >= kDuration) {
		count_ = kDuration;
		isFinished_ = true;
	}

	// フェード処理：透明度を 1.0 → 0.0 に
	color_.w = 1.0f - (count_ / kDuration); // w = alpha
	

	// 行列を定数バッファに転送
	worldTransform_.UpdateMatrix();
	// 色変更オブジェクトに色の数値を設定する
	objectColor_.SetColor(color_);
}

void Effect::Draw(Camera& camera) {
	// 3Dモデルを描画
	model_->Draw(worldTransform_, camera, &objectColor_);
}
