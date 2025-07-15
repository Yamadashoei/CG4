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
	KamataEngine::Sprite* bg1_ = nullptr;
	KamataEngine::Sprite* bg2_ = nullptr;

	float scrollX_ = 0.0f;
};
