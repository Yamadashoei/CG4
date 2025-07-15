#include "TitleScene.h"

using namespace KamataEngine;

TitleScene::TitleScene() {}

TitleScene::~TitleScene() {
	delete titleSprite_;
	delete enterKeySprite_;
}

void TitleScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();

	uint32_t titleTex = TextureManager::Load("./Resources/title.png");
	titleSprite_ = Sprite::Create(titleTex, {0.0f, 0.0f});

	uint32_t enterTex = TextureManager::Load("./Resources/enter.png");
	enterKeySprite_ = Sprite::Create(enterTex, {400.0f, 480.0f});
}

void TitleScene::Update() {
	if (input_->TriggerKey(DIK_RETURN)) {
		isSceneEnd_ = true;
	}
}

void TitleScene::Draw() {
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Sprite::PreDraw(commandList);
	titleSprite_->Draw();
	enterKeySprite_->Draw();
	Sprite::PostDraw();
}

bool TitleScene::IsSceneEnd() const { return isSceneEnd_; }
