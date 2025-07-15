#include "Stage.h"

using namespace KamataEngine;

Stage::Stage() {}

Stage::~Stage() {
	delete bg1_;
	delete bg2_;
}

void Stage::Initialize() {
	// 背景画像読み込み
	uint32_t tex = TextureManager::Load("./Resources/bg.png");

	// 2枚用意して横に並べる
	bg1_ = Sprite::Create(tex, {0.0f, 0.0f});
	bg2_ = Sprite::Create(tex, {1280.0f, 0.0f}); 
}

void Stage::Update() {
	// 左方向にスクロール
	scrollX_ -= 2.0f;

	// ループ処理
	if (scrollX_ <= -1280.0f) {
		scrollX_ += 1280.0f;
	}

	// 背景2枚の位置を更新
	bg1_->SetPosition({scrollX_, 0.0f});
	bg2_->SetPosition({scrollX_ + 1280.0f, 0.0f});
}

void Stage::Draw() {
	ID3D12GraphicsCommandList* commandList = DirectXCommon::GetInstance()->GetCommandList();

	Sprite::PreDraw(commandList);
	bg1_->Draw();
	bg2_->Draw();
	Sprite::PostDraw();
}
