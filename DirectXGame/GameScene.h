#pragma once
#include "KamataEngine.h"
#include "Particle.h"
#include "Stage.h"

#include <3d/Camera.h>
#include <3d/Model.h>

class GameScene {
public:
	GameScene();
	~GameScene();

	void Initialize();
	void Update();
	void Draw();

private:
	KamataEngine::DirectXCommon* dxCommon_ = nullptr;
	KamataEngine::Input* input_ = nullptr;
	KamataEngine::Audio* audio_ = nullptr;

	KamataEngine::Model* modelParticle_ = nullptr;
	KamataEngine::Camera camera_;
	Particle* particle_ = nullptr;

	Stage* stage_ = nullptr;

};
