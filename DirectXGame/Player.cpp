#include "Player.h"

using namespace KamataEngine;

Player::Player() {}
Player::~Player() {}

void Player::Initialize(Model* model, Vector3 position, Vector3 velocity) {
	assert(model);
	playerModel_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.scale_ = {3.0f, 3.0f, 3.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	velocity_ = velocity;

	worldTransform_.UpdateMatrix();
}

void Player::Update() {
	// 例：移動処理（必要に応じて削除/変更可）
	worldTransform_.translation_ += velocity_;

	worldTransform_.UpdateMatrix();
}

void Player::Draw(const Camera& camera) {
	
		playerModel_->Draw(worldTransform_, camera);
	
}
