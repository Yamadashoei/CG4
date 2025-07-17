#include "GameScene.h"

using namespace KamataEngine;

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete modelParticle_;
	delete particle_;
	delete stage_;
}

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// モデルの生成
	modelParticle_ = Model::CreateSphere(4, 4);
	// カメラの初期化
	camera_.Initialize();

	// パーティクルの生成
	particle_ = new Particle();
	// パーティクルの初期化
	particle_->Initialize(modelParticle_);

	// 背景スクロールの初期化
	stage_ = new Stage();
	stage_->Initialize();
}

void GameScene::Update() {
	/// パーティクルの更新
	particle_->Update();
	// 背景スクロールの更新
	stage_->Update();
}

void GameScene::Draw() {
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	// 背景スプライト
	Sprite::PreDraw(commandList);

	stage_->Draw();

	Sprite::PostDraw();
	dxCommon_->ClearDepthBuffer();

	// 3Dオブジェクト
	Model::PreDraw(commandList);
	particle_->Draw(camera_);
	Model::PostDraw();

	// 前景スプライト
	Sprite::PreDraw(commandList);
	Sprite::PostDraw();
}
