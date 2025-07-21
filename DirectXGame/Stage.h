#pragma once

#include "KamataEngine.h"

class Stage {
public:
	Stage();
	~Stage();

	void Initialize();
	void Update();
	void Draw();

private:
	KamataEngine::DirectXCommon* dxCommon_ = nullptr;
	KamataEngine::Input* input_ = nullptr;

	KamataEngine::Sprite* bgLeft_ = nullptr;
	KamataEngine::Sprite* bgRight_ = nullptr;

	float scrollX_ = 0.0f;
	const float scrollSpeed_ = 2.0f;
	const float bgWidth_ = 1280.0f;
};
