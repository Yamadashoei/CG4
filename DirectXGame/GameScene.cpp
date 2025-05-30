#include "GameScene.h"
#include <random>

using namespace KamataEngine;
using namespace MathUtility;

// ランダム初期化
std::random_device seedGen;
std::mt19937 engine(seedGen());
std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
// std::uniform_real_distribution<float> scaleXDist(0.03f, 0.1f);    // 太さ（横）
// std::uniform_real_distribution<float> scaleYDist(0.5f, 1.0f);     // 長さ（縦）
// std::uniform_real_distribution<float> rotOffsetDist(0.0f, 3.14f); // 少しだけブレさせる
std::uniform_real_distribution<float> offsetDist(-0.5f, 0.5f); // 爆発位置

GameScene::~GameScene() {
	delete modelEffect_;
	for (Effect* e : effects_)
		delete e;
	effects_.clear();
}

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// モデルの初期化
	modelEffect_ = Model::CreateFromOBJ("bakuhatu", true);

	// カメラ設定
	camera_.Initialize();
	camera_.translation_.z = -10.0f;
	camera_.UpdateMatrix();

	//
	for (int i = 0; i < 5; ++i) {
		Vector3 scale = {0.5f, 0.5f, 0.5f};
		Vector3 rotation = {0.0f, 0.0f, 0.0f};
		Vector3 offset = {offsetDist(engine), offsetDist(engine), offsetDist(engine)};
		Vector3 position = {0.0f, 0.0f, 0.0f};
		Effect* effect = new Effect();
		effect->Initialize(modelEffect_, scale, rotation, position);
		effects_.push_back(effect);
	}
}

void GameScene::Update() {
	if (rand() % 10 == 0) {
		// 位置
		Vector3 position = {distribution(engine) * 5.0f, distribution(engine) * 3.0f, 0.0f};
		EffectBorn(position);
	}
	for (Effect* e : effects_)
		e->Update();
	effects_.remove_if([](Effect* effect) {
		if (effect->GetDeathFlag()) {
			delete effect;
			return true;
		}
		return false;
	});
}

void GameScene::Draw() {
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	///

	for (Effect* e : effects_) {
		e->Draw(camera_);
	}
	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::EffectBorn(KamataEngine::Vector3 position) {
	for (int i = 0; i < 5; i++) {
		// エフェクトの初期化
		Effect* effect = new Effect();
		Vector3 scale = {0.5f, 0.5f, 0.5f};
		Vector3 rotation = {0.0f, 0.0f, 0.0f};
		Vector3 offset = {offsetDist(engine), offsetDist(engine), offsetDist(engine)};
		Vector3 offsetPos = position + offset;
		effect->Initialize(modelEffect_, scale, rotation, position);
		effects_.push_back(effect);
	}
}
