#include "Effect.h"
#include <algorithm>
#include <cassert>
#include <random>

using namespace KamataEngine;
using namespace MathUtility;

std::random_device random;
std::mt19937 origin(random());
std::uniform_real_distribution<float> velocity(-0.05f, 0.05f);

std::uniform_real_distribution<float> randColor(0.0f, 1.0f);

void Effect::Initialize(Model* model, Vector3 scale, Vector3 rotation, Vector3 position) {
	assert(model);
	model_ = model;

	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.scale_ = scale;
	worldTransform_.rotation_ = rotation;
	worldTransform_.translation_ = position;

	// 色の設定
	objectColor_.Initialize();
	//  赤かオレンジをランダムに選択
	if (randColor(origin) < 0.5f) {
		color_ = {1.0f, 0.1f, 0.0f, 1.0f}; // 赤
	} else {
		color_ = {1.0f, 0.25f, 0.0f, 1.0f}; // オレンジ
	}
	// color_ = {1.0f, 0.1f, 0.0f, 1.0f};

	oriScale_ = scale;
	velocity_ = {velocity(origin), velocity(origin) + 0.05f, velocity(origin)};
}

void Effect::Update() {
	// 終了ならリターン
	if (isFinished_)
		return;
	// カウンターを1フレーム分の秒数進める
	count_ += 1.0f / 60.0f;
	if (count_ >= kDuration) {
		count_ = kDuration;
		isFinished_ = true;
	}

	// フェード処理(透明度 1.0 → 0.0)
	color_.w = 1.0f - (count_ / kDuration);

	float expansion = 1.5f - (count_ / kDuration) * 1.0f;
	worldTransform_.scale_ = oriScale_ * expansion;
	worldTransform_.translation_ = worldTransform_.translation_ + velocity_;

	// 行列を定数バッファに転送
	worldTransform_.UpdateMatrix();
	// 色変更オブジェクトに色の数値を設定する
	objectColor_.SetColor(color_);
}

void Effect::Draw(Camera& camera) {
	// 3Dモデルを描画
	model_->Draw(worldTransform_, camera, &objectColor_);
}