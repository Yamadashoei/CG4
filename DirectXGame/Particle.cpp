#include "Particle.h"
#include <cassert>
using namespace KamataEngine;

void Particle::Initialize(KamataEngine::Model* model) {
	// Nullチェック
	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録する
	modelParticle_ = model;
	// ワールド変換の初期化
	worldTransform_.Initialize();
}

void Particle::Update() {
	// 行数を定数バッファに転送
	worldTransform_.TransferMatrix();
}

void Particle::Draw(Camera& camera) {
	// 3Dモデルの描画
	modelParticle_->Draw(worldTransform_, camera);
}
