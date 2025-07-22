#include "GameScene.h"

using namespace KamataEngine;

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete modelParticle_;
	delete modelPlayer_;
	delete particle_;
	delete stage_;
	delete player_;
	delete graph_;
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

	// パーティクル初期化
	particle_ = new Particle();
	particle_->Initialize(modelParticle_);

	// ステージ初期化
	stage_ = new Stage();
	stage_->Initialize();

	// プレイヤー初期化
	player_ = new Player();
	player_->Initialize(modelPlayer_, {-2.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f});

	// グラフ初期化
	graph_ = new Graph();
	graph_->Initialize(200.0f, 20.0f, Vector2{100, 50}); // Vector2 は KamataEngine 名前空間内でもOK
}

void GameScene::Update() {

	player_->Update();
	stage_->Update();
	particle_->Update();

	// HPを減らして0になったらまた100に戻す
	if (hp > 0) {
		hp--;
	} else {
		hp = 100; // 0になったらリセット
	}

	// グラフ更新
	float hpRate = static_cast<float>(hp) / maxHp;
	graph_->SetTargetRate(hpRate);
	graph_->Update();
}

void GameScene::Draw() {
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	// 背景スプライト
	Sprite::PreDraw(commandList);
	stage_->Draw();
	Sprite::PostDraw();

	// 深度バッファをクリア
	dxCommon_->ClearDepthBuffer();

	// 3Dオブジェクト描画
	Model::PreDraw(commandList);
	player_->Draw(camera_);
	// particle_->Draw(camera_); // 使用していない場合はコメントでもOK
	Model::PostDraw();

	// 前景スプライト（グラフ表示）
	Sprite::PreDraw(commandList);
	graph_->Draw();
	Sprite::PostDraw();
}
