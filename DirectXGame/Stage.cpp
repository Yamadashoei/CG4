#include "Stage.h"
#include "kMath.h"

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
	bgRight_ = Sprite::Create(tex2, {bgWidth_, 0});
}

void Stage::Update() {
	// 現在の位置を取得
	Vector2 leftPos = bgLeft_->GetPosition();
	Vector2 rightPos = bgRight_->GetPosition();

	// 左へスクロール
	leftPos.x -= scrollSpeed_;
	rightPos.x -= scrollSpeed_;

	// 位置を更新
	bgLeft_->SetPosition(leftPos);
	bgRight_->SetPosition(rightPos);

	// bgLeft_ が左端を完全に超えたら右側に回す
	if (bgLeft_->GetPosition().x <= -bgWidth_) {
		bgLeft_->SetPosition({bgRight_->GetPosition().x + bgWidth_, 0.0f});
		std::swap(bgLeft_, bgRight_);
	}


}


void Stage::Draw() {
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
	Sprite::PreDraw(cmdList);

	bgLeft_->Draw();
	bgRight_->Draw();

	Sprite::PostDraw();
}
