#include "Stage.h"
using namespace KamataEngine;

void Stage::Initialize(Model* model, Camera* camera, const Vector3& center, float startRadius, float endRadius, float durationSec) {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	model_ = model;
	cameraRef_ = camera;

	center_ = center;
	startR_ = startRadius;
	endR_ = endRadius;
	duration_ = (durationSec <= 0.01f) ? 0.01f : durationSec;

	radius_ = startR_;
	t_ = 0.0f;

	worldTransform_.Initialize();
	// モデルは半径=1・高さ=1想定。XZに半径を入れる。
	worldTransform_.scale_ = {radius_, 1.0f, radius_};
	worldTransform_.translation_ = center_;
	worldTransform_.UpdateMatrix();
}

void Stage::Update() {
	t_ += kFixedDt_ / duration_;
	if (t_ > 1.0f)
		t_ = 1.0f;

	float u = Ease(t_);
	radius_ = startR_ + (endR_ - startR_) * u;

	worldTransform_.scale_.x = radius_;
	worldTransform_.scale_.z = radius_;
	worldTransform_.translation_ = center_;
	worldTransform_.UpdateMatrix();
}

void Stage::Draw() {
	if (!model_ || !cameraRef_)
		return;
	Vector4 color{1, 1, 1, 1};
	model_->Draw(worldTransform_, *cameraRef_, textureHandle_);
}

void Stage::SetPosition(const Vector3& pos) {
	center_ = pos; // ★ center_ も更新しておく
	worldTransform_.translation_ = pos;
	worldTransform_.UpdateMatrix();
}
