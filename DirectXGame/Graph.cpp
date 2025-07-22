#include "Graph.h"
#include <algorithm>
#include <base/TextureManager.h>

using namespace KamataEngine;

void Graph::Initialize(float maxWidth, float height, const Vector2& position) {
	maxWidth_ = maxWidth;
	height_ = height;

	uint32_t whiteTex = TextureManager::Load("./Resources/white1x1.png");

	// 背景（赤・半透明）
	background_ = Sprite::Create(whiteTex, position);
	background_->SetSize({maxWidth_, height_});
	background_->SetColor({1, 0, 0, 0.5f});

	// 緑バー（透明度なし）
	bar_ = Sprite::Create(whiteTex, position);
	bar_->SetSize({maxWidth_, height_});
	bar_->SetColor({0, 1, 0, 1});

	currentRate_ = 1.0f;
	targetRate_ = 1.0f;
}

void Graph::SetTargetRate(float rate) { targetRate_ = std::clamp(rate, 0.0f, 1.0f); }

void Graph::Update() {
	// 徐々にゲージを近づける
	float speed = 0.1f;

	// 減少は徐々に、増加は即反映
	if (targetRate_ < currentRate_) {
		currentRate_ += (targetRate_ - currentRate_) * speed;
	} else {
		currentRate_ = targetRate_;
	}

	bar_->SetSize({maxWidth_ * currentRate_, height_});
}

void Graph::Draw() {
	background_->Draw();
	bar_->Draw();
}
