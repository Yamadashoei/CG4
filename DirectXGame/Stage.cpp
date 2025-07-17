#include "Stage.h"

using namespace KamataEngine;

//あと反転のみ

Stage::Stage() {}

Stage::~Stage() {
	delete bgSprite1_;
	delete bgSprite2_;
}

void Stage::Initialize() {
	uint32_t tex = TextureManager::Load("./Resources/bg.png");

	bgSprite1_ = Sprite::Create(tex, {0, 0});
	bgSprite1_->SetSize({bgWidth_, 720.0f});

	bgSprite2_ = Sprite::Create(tex, {bgWidth_, 0});
	bgSprite2_->SetSize({bgWidth_, 720.0f});
}

void Stage::Update() {
	scrollX_ -= scrollSpeed_;

	if (scrollX_ <= -bgWidth_) {
		scrollX_ += bgWidth_;
	}

	// 位置更新
	bgSprite1_->SetPosition({scrollX_, 0});
	bgSprite2_->SetPosition({scrollX_ + bgWidth_, 0});

	//// 1枚目は通常、2枚目は反転
	//bgSprite1_->SetIsFlipX(false);
	//bgSprite2_->SetIsFlipX(true); // ←ここで反転！
	bgSprite1_->SetPosition({scrollX_, 0.0f});
	bgSprite2_->SetPosition({scrollX_ + 1280.0f, 0.0f});

}


void Stage::Draw() {
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
	Sprite::PreDraw(cmdList);

	bgSprite1_->Draw();
	bgSprite2_->Draw();

	Sprite::PostDraw();
}
