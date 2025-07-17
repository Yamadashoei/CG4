#include "Stage.h"

using namespace KamataEngine;

Stage::Stage() {}

Stage::~Stage() {
	delete bgLeft_;
	delete bgRight_;
}

void Stage::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();

	uint32_t tex = TextureManager::Load("./Resources/bg.png");

	// 左側背景（通常）
	bgLeft_ = Sprite::Create(tex, {0.0f, 0.0f});
	bgLeft_->SetSize({1280.0f, 720.0f});

	// 右側背景（反転 + アンカーポイント調整）
	bgRight_ = Sprite::Create(tex, {1280.0f, 0.0f});
	bgRight_->SetSize({1280.0f, 720.0f});
	bgRight_->SetAnchorPoint({1.0f, 0.0f}); // 右上を基準に
	bgRight_->SetIsFlipX(true);
}

void Stage::Update() {
	// 左へスクロール
	scrollX_ -= scrollSpeed_;

	if (scrollX_ <= -1280.0f) {
		scrollX_ += 1280.0f;
	}

	// 位置更新
	bgLeft_->SetPosition({scrollX_, 0.0f});
	bgRight_->SetPosition({scrollX_ + 1280.0f, 0.0f});
}

void Stage::Draw() {
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	Sprite::PreDraw(commandList);

	bgLeft_->Draw();
	bgRight_->Draw();

	Sprite::PostDraw();
}
