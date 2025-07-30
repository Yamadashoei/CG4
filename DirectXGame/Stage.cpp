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

	uint32_t tex1 = TextureManager::Load("./Resources/scene/bg1.png");
	uint32_t tex2 = TextureManager::Load("./Resources/scene/bg2.png");


	// 左側背景
	bgLeft_ = Sprite::Create(tex1, {0.0f, 0.0f});
	bgLeft_->SetSize({1280.0f, 720.0f});

	// 右側背景
	bgRight_ = Sprite::Create(tex2, {1280.0f, 0.0f});
	bgRight_->SetSize({1280.0f, 720.0f});

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
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	Sprite::PreDraw(commandList);

	bgLeft_->Draw();
	bgRight_->Draw();

	Sprite::PostDraw();
	
}
