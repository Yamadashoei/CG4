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

	modelParticle_ = Model::CreateSphere(4, 4);
	camera_.Initialize();

	particle_ = new Particle();
	particle_->Initialize(modelParticle_);

	stage_ = new Stage();
	stage_->Initialize();
}

void GameScene::Update() {
	particle_->Update();
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
