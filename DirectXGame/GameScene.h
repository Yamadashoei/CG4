#pragma once
#include "KamataEngine.h"

#include <3d/Camera.h>
#include <3d/Model.h>
#include <3d/WorldTransform.h>
#include <Windows.h>
#include <base/DirectXCommon.h>

#include "Stage.h"
#include "kMath.h"

class GameScene {
public:
	GameScene();
	~GameScene();

	void Initialize();
	void Update();
	void Draw();

	void UpdateCameraControls_(float dt);

private:
	// --- カメラ操作 ---
	float camYaw_ = 0.0f;       // 左右回転（Y軸）
	float camPitch_ = 0.6f;     // 上下回転（X軸） 俯瞰気味の初期値
	float camSpeed_ = 12.0f;    // 通常移動速度(m/s)
	float camBoost_ = 3.0f;     // Shiftでの倍率
	float mouseSens_ = 0.0025f; // マウス感度（ラジアン/ピクセル）

	bool rmbHeld_ = false;    // 右ボタン押下中か
	POINT prevMousePos_{};    // 右ドラッグ計算用
	float wheelAccum_ = 0.0f; // ホイール移動蓄積（任意）

private:
	KamataEngine::DirectXCommon* dxCommon_ = nullptr;
	KamataEngine::Input* input_ = nullptr;
	KamataEngine::Audio* audio_ = nullptr;

	KamataEngine::Camera camera_;

	Stage* stage_ = nullptr;
	KamataEngine::Model* modelStage_ = nullptr;

	float fixedDt_ = 1.0f / 60.0f; // 今は未使用
};
