#include "TitleScene.h"

using namespace KamataEngine;

TitleScene::TitleScene() {}

TitleScene::~TitleScene() {
	delete background_;
	delete titleSprite_;
	delete enterKeySprite_;
}

void TitleScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();

	// 背景画像
	uint32_t bgTex = TextureManager::Load("./Resources/title.png");
	background_ = Sprite::Create(bgTex, {0.0f, 0.0f});

	// タイトル文字
	uint32_t titleTex = TextureManager::Load("./Resources/titleTex.png");
	titleSprite_ = Sprite::Create(titleTex, {400.0f, titleY_});

	// Hit Enter Key の文字
	uint32_t enterTex = TextureManager::Load("./Resources/enter.png");
	enterKeySprite_ = Sprite::Create(enterTex, {490.0f, 500.0f});
}

void TitleScene::Update() {
	frameCount_++;

	// タイトルのY軸移動（上から落下）
	if (titleY_ < 150.0f) {
		titleY_ += 2.0f; // 落下速度
		titleSprite_->SetPosition({400.0f, titleY_});
	}

	// Enterキー押下でシーン遷移
	if (input_->TriggerKey(DIK_RETURN)) {
		isSceneEnd_ = true;
	}
}

void TitleScene::Draw() {
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Sprite::PreDraw(commandList);

	background_->Draw();
	titleSprite_->Draw();

	// Hit Enter Key の点滅表示
	if ((frameCount_ % 60) < 30) {
		enterKeySprite_->Draw();
	}

	Sprite::PostDraw();
	dxCommon_->ClearDepthBuffer();

}

bool TitleScene::IsSceneEnd() const { return isSceneEnd_; }
