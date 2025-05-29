#include "GameScene.h"
#include <random>

using namespace KamataEngine;
using namespace MathUtility;

std::random_device seedGen;
std::mt19937 engine(seedGen());
std::uniform_real_distribution<float> distrubution(-1.0f, 1.0f);

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

	modelEffect_ = Model::CreateFromOBJ("bakuhatu", true);

	camera_.Initialize();
	camera_.translation_.z = -10.0f;
	camera_.UpdateMatrix();

	for (int i = 0; i < 5; ++i) {
		Vector3 scale = {0.5f, 0.5f, 0.5f};
		Vector3 rotation = {0.0f, 0.0f, 0.0f};
		Vector3 position = {0.0f, 0.0f, 0.0f};
		Effect* effect = new Effect();
		effect->Initialize(modelEffect_, scale, rotation, position);
		effects_.push_back(effect);
	}
}

void GameScene::Update() {
	if (rand() % 10 == 0) {
		Vector3 position = {distrubution(engine) * 5.0f, distrubution(engine) * 3.0f, 0.0f};
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

	Sprite::PreDraw(commandList);
	Sprite::PostDraw();
	dxCommon_->ClearDepthBuffer();

	Model::PreDraw(commandList);
	for (Effect* e : effects_)
		e->Draw(camera_);
	Model::PostDraw();

	Sprite::PreDraw(commandList);
	Sprite::PostDraw();
}

void GameScene::EffectBorn(KamataEngine::Vector3 position) {
	for (int i = 0; i < 5; i++) {
		Effect* effect = new Effect();
		Vector3 scale = {0.5f, 0.5f, 0.5f};
		Vector3 rotation = {0.0f, 0.0f, 0.0f};
		effect->Initialize(modelEffect_, scale, rotation, position);
		effects_.push_back(effect);
	}
}
