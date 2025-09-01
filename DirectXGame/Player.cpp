#include "Player.h"
#include <algorithm>
#include <cassert>

using namespace KamataEngine;

Player::Player() {}
Player::~Player() {}

//プレイヤー初期化
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
	Input* input = Input::GetInstance();

	float moveSpeed = 0.2f;

	// 入力に応じて移動
	if (input->PushKey(DIK_D)) {
		worldTransform_.translation_.x += moveSpeed;
	}
	if (input->PushKey(DIK_A)) {
		worldTransform_.translation_.x -= moveSpeed;
	}
	if (input->PushKey(DIK_W)) {
		worldTransform_.translation_.y += moveSpeed;
	}
	if (input->PushKey(DIK_S)) {
	
	worldTransform_.translation_.y -= moveSpeed;
	}

	// === ワールド座標上での範囲制限 ===
	worldTransform_.translation_.x = std::clamp(worldTransform_.translation_.x, -35.0f, 35.0f);
	worldTransform_.translation_.y = std::clamp(worldTransform_.translation_.y, -18.0f, 18.0f);

	worldTransform_.UpdateMatrix();
}


void Player::Draw(const Camera& camera) {
	
		playerModel_->Draw(worldTransform_, camera);
	
}
