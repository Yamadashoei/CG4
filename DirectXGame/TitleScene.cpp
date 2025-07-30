#include "TitleScene.h"

using namespace KamataEngine;

TitleScene::TitleScene() {}

TitleScene::~TitleScene() {
	delete background_;
	delete titleSprite_;
	delete enterKeySprite_;
	delete blackSprite_;
}

void TitleScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();

	// 背景画像
	uint32_t bgTex = TextureManager::Load("./Resources/scene/title.png");
	background_ = Sprite::Create(bgTex, {0.0f, 0.0f});

	// タイトル文字
	uint32_t titleTex = TextureManager::Load("./Resources/scene/titleTex.png");
	titleSprite_ = Sprite::Create(titleTex, {400.0f, titleY_});

	// Hit Enter Key の文字
	uint32_t enterTex = TextureManager::Load("./Resources/scene/enter.png");
	enterKeySprite_ = Sprite::Create(enterTex, {490.0f, 500.0f});

	// フェード用の黒スプライト作成（全画面に拡大）
	uint32_t blackTex = TextureManager::Load("./Resources/black1x1.png");
	blackSprite_ = Sprite::Create(blackTex, {0.0f, 0.0f});
	blackSprite_->SetSize({1280.0f, 720.0f}); // 画面サイズに合わせる
}

void TitleScene::Update() {
	frameCount_++;

	// タイトルのY軸移動（上から落下）
	if (titleY_ < 150.0f) {
		titleY_ += 2.0f; // 落下速度
		titleSprite_->SetPosition({400.0f, titleY_});
	}

	// Enterキー押下でフェード開始
	if (!startTransition_ && input_->TriggerKey(DIK_RETURN)) {
		startTransition_ = true;
		transitionFrameCount_ = 0;
	}

	// フェードアウト処理
	if (startTransition_) {
		transitionFrameCount_++;
		fadeAlpha_ += 0.01f;

		if (fadeAlpha_ >= 1.0f) {
			fadeAlpha_ = 1.0f;
			isSceneEnd_ = true; // フェード完了後にシーン切り替え
		}
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

	// フェードアウト描画
	if (startTransition_) {
		blackSprite_->SetColor({0.0f, 0.0f, 0.0f, fadeAlpha_});
		blackSprite_->Draw();
	}

	Sprite::PostDraw();
	dxCommon_->ClearDepthBuffer();
}

bool TitleScene::IsSceneEnd() const { return isSceneEnd_; }
