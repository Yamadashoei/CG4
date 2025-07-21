#include "GameScene.h"

using namespace KamataEngine;

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete modelParticle_;
	delete modelPlayer_;
	delete particle_;
	delete stage_;
	delete player_;
}

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// モデル生成
	modelParticle_ = Model::CreateSphere(4, 4);
	modelPlayer_ = Model::CreateFromOBJ("player");
	// カメラの初期化
	camera_.Initialize();

	// パーティクルの生成
	particle_ = new Particle();
	// パーティクルの初期化
	particle_->Initialize(modelParticle_);

	// 背景スクロールの初期化
	stage_ = new Stage();
	stage_->Initialize();

	// プレイヤーの初期化
	player_ = new Player();
	// プレイヤーのモデルを設定(modelPlayer_,位置,速度)
	player_->Initialize(modelPlayer_, {-2.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f});
}

void GameScene::Update() {
	// プレイヤーの更新
	player_->Update();
	/// パーティクルの更新
	particle_->Update();
	// 背景スクロールの更新
	stage_->Update();

	//// プレイヤーにカメラを追従させる
	//camera_.translation_.x = player_->GetPosition().x;
	//camera_.UpdateMatrix();

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
	//particle_->Draw(camera_);
	player_->Draw(camera_);
	Model::PostDraw();

	// 前景スプライト
	Sprite::PreDraw(commandList);
	Sprite::PostDraw();
}
