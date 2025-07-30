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
	for (int i = 0; i < 5; i++) {
		delete numberSprite_[i];
	}
}

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// モデル生成
	modelParticle_ = Model::CreateSphere(4, 4);
	modelPlayer_ = Model::CreateFromOBJ("player");
	numberTextureHandle_ = TextureManager::Load("./Resources/number.png"); // 数字画像の読み込み

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
	graph_->Initialize(200.0f, 20.0f, Vector2{100, 80});

	// スプライト5つを生成
	for (int i = 0; i < 5; i++) {
		numberSprite_[i] = Sprite::Create(numberTextureHandle_, {100.0f + numberSize_.x * i, 5.0f});
		numberSprite_[i]->SetSize(numberSize_);
	}
}

void GameScene::Update() {

	player_->Update();
	stage_->Update();
	particle_->Update();

	// グラフ更新
	hp--;
	if (hp < 0) {
		hp = 100;
	}
	float hpRate = static_cast<float>(hp) / maxHp;
	graph_->SetTargetRate(hpRate);
	graph_->Update();

	// 数値カウントアップ
	score++;
	if (score > 99999) {
		score = 0;
	}
	// 数値表示の更新
	int number = score;
	int digit = 10000;

	for (int i = 0; i < 5; i++) {
		int nowNumber = number / digit % 10;
		numberSprite_[i]->SetTextureRect({numberSize_.x * nowNumber, 0}, numberSize_);
		digit /= 10;
	}
}

void GameScene::Draw() {
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	// 背景スプライト
	Sprite::PreDraw(commandList);
	// ステージの背景
	stage_->Draw();
	Sprite::PostDraw();

	// 深度バッファをクリア
	dxCommon_->ClearDepthBuffer();

	// 3Dオブジェクト描画
	Model::PreDraw(commandList);
	// プレイヤー描画
	player_->Draw(camera_);
	// particle_->Draw(camera_);
	Model::PostDraw();

	// 前景スプライト（グラフ表示）
	Sprite::PreDraw(commandList);
	// グラフの背景
	graph_->Draw();
	// 数値表示
	for (int i = 0; i < 5; i++) {
		numberSprite_[i]->Draw();
	}

	Sprite::PostDraw();
}
