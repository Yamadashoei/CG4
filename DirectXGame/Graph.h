#pragma once
#include <2d/Sprite.h>
#include <math/Vector2.h>

class Graph {
public:
	// 初期化（最大幅, 高さ, 表示位置）
	void Initialize(float maxWidth, float height, const KamataEngine::Vector2& position);

	// 割合をセット
	void SetTargetRate(float rate);

	// 更新
	void Update();

	// 描画
	void Draw();

private:
	KamataEngine::Sprite* background_ = nullptr;
	KamataEngine::Sprite* bar_ = nullptr;

	float maxWidth_ = 0.0f;
	float height_ = 0.0f;

	float currentRate_ = 1.0f; // 表示中の割合
	float targetRate_ = 1.0f;  // 減らしたい目標割合
};
