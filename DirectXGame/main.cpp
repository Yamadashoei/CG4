#include "GameScene.h"
#include "kMath.h" // Vector3演算子とNormalize
#include <Windows.h>
#include <cmath>

using namespace KamataEngine;

GameScene::GameScene() {}
GameScene::~GameScene() {
	delete stage_;
}

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// カメラ
	camera_.Initialize();
	camYaw_ = 0.0f;
	camPitch_ = 0.8f;
	camera_.rotation_.x = camPitch_;
	camera_.rotation_.y = camYaw_;
	camera_.translation_ = {0.0f, 25.0f, -25.0f};
	camera_.UpdateMatrix();

	// モデル
	modelStage_ = Model::CreateFromOBJ("stage");

	// ステージ
	stage_ = new Stage();
	Vector3 center{0, 0, 0};
	stage_->Initialize(
	    modelStage_, &camera_, center,
	    /*startR*/ 18.0f, /*endR*/ 3.0f, /*durSec*/ 60.0f);
}

void GameScene::Update() {
	UpdateCameraControls_(fixedDt_);
	stage_->Update();
}

void GameScene::Draw() {
	ID3D12GraphicsCommandList* cl = dxCommon_->GetCommandList();

	Sprite::PreDraw(cl);
	Sprite::PostDraw();
	dxCommon_->ClearDepthBuffer();

	Model::PreDraw(cl);
	stage_->Draw();
	Model::PostDraw();

	Sprite::PreDraw(cl);
	Sprite::PostDraw();
}

void GameScene::UpdateCameraControls_(float dt) {
	float speed = camSpeed_;
	if (input_->PushKey(DIK_LSHIFT) || input_->PushKey(DIK_RSHIFT))
		speed *= camBoost_;

	float cy = std::cos(camYaw_), sy = std::sin(camYaw_);
	Vector3 fwd{sy, 0.0f, cy};
	Vector3 right{cy, 0.0f, -sy};

	Vector3 move{0, 0, 0};
	if (input_->PushKey(DIK_W))
		move += fwd;
	if (input_->PushKey(DIK_S))
		move += (fwd * -1.0f); // operator-= 未実装でもOKに
	if (input_->PushKey(DIK_D))
		move += right;
	if (input_->PushKey(DIK_A))
		move += (right * -1.0f);
	if (input_->PushKey(DIK_E))
		move.y += 1.0f;
	if (input_->PushKey(DIK_Q))
		move.y -= 1.0f;

	move = Normalize(move);
	camera_.translation_ += move * (speed * dt);

	SHORT rmb = GetAsyncKeyState(VK_RBUTTON);
	bool rmbNow = (rmb & 0x8000) != 0;

	POINT cur;
	GetCursorPos(&cur);
	if (rmbNow) {
		static bool first = true;
		if (!rmbHeld_) {
			rmbHeld_ = true;
			prevMousePos_ = cur;
			first = false;
		} else {
			LONG dx = cur.x - prevMousePos_.x;
			LONG dy = cur.y - prevMousePos_.y;
			prevMousePos_ = cur;

			camYaw_ += static_cast<float>(dx) * mouseSens_;
			camPitch_ += static_cast<float>(dy) * mouseSens_;
			const float kMinPitch = -1.55f, kMaxPitch = 1.55f;
			if (camPitch_ < kMinPitch)
				camPitch_ = kMinPitch;
			if (camPitch_ > kMaxPitch)
				camPitch_ = kMaxPitch;
		}
	} else {
		rmbHeld_ = false;
	}

	camera_.rotation_.x = camPitch_;
	camera_.rotation_.y = camYaw_;
	camera_.UpdateMatrix();
}
