#include "GameScene.h"

using namespace KamataEngine;

GameScene::GameScene() {}

GameScene::~GameScene() { Model2::StaticFinalize(); }

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	// カメラの初期化
	camera_.Initialize();

	// WorldTransformの初期化
	worldTransform_.Initialize();
	// サイズ変更
	worldTransform_.scale_ = {3.0f, 3.0f, 1.0f};
	// 行列更新
	worldTransform_.UpdateMatrix();

	// テクスチャ読み込み
	textureHandle_ = TextureManager::GetInstance()->Load("./Resources/uvChecker.png");

	// モデルの初期化
	Model2::StaticInitialize();
	// Model2(cubeモデル)
	model2_ = Model2::Create();
	// Model2の四角形モデルを生成
	model2_ = Model2::CreateSquare();
}

void GameScene::Update() {}

void GameScene::Draw() {
	// DirectXCommon インスタンスの取得
	// DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model2::PreDraw(commandList);

	model2_->Draw(worldTransform_, camera_, textureHandle_);

	// 3Dオブジェクト描画後処理
	Model2::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}