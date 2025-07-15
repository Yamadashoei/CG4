#pragma once
#include "KamataEngine.h"

class TitleScene {
public:
	TitleScene();
	~TitleScene();

	void Initialize();
	void Update();
	void Draw();

	bool IsSceneEnd() const;

private:
	KamataEngine::DirectXCommon* dxCommon_ = nullptr;
	KamataEngine::Input* input_ = nullptr;

	KamataEngine::Sprite* background_ = nullptr;
	KamataEngine::Sprite* titleSprite_ = nullptr;
	KamataEngine::Sprite* enterKeySprite_ = nullptr;

	float titleY_ = -100.0f; // タイトルのY位置
	int frameCount_ = 0;     // フレームカウント

	bool isSceneEnd_ = false;
};
