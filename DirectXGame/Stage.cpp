#include "Stage.h"

using namespace KamataEngine;

Stage::Stage() {}

Stage::~Stage() {
	delete bgLeft_;
	delete bgRight_;
}

void Stage::Initialize() {
	uint32_t tex1 = TextureManager::Load("./Resources/bg1.png");
	uint32_t tex2 = TextureManager::Load("./Resources/bg2.png");

	bgLeft_ = Sprite::Create(tex1, {0, 0});
	bgLeft_->SetSize({bgWidth_, 720.0f});

	bgRight_ = Sprite::Create(tex2, {0, 0});
	bgRight_->SetSize({bgWidth_, 720.0f});
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
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
	Sprite::PreDraw(cmdList);

	bgLeft_->Draw();
	bgRight_->Draw();

	Sprite::PostDraw();
}
