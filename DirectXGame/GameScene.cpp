#include "GameScene.h"
#include <Windows.h> // マウス入力用
#include <cmath>

using namespace KamataEngine;

GameScene::~GameScene() {
    delete stage_;
}

void GameScene::Initialize() {
    dxCommon_ = DirectXCommon::GetInstance();
    input_    = Input::GetInstance();
    audio_    = Audio::GetInstance();

    // --- カメラ初期化（俯瞰気味の初期位置＆角度） ---
    camera_.Initialize();
    camYaw_   = 0.0f;
    camPitch_ = 0.8f;                           // ≒46° 下向き
    camera_.rotation_.x  = camPitch_;
    camera_.rotation_.y  = camYaw_;
    camera_.translation_ = { 0.0f, 25.0f, -25.0f };
    camera_.UpdateMatrix();

    // --- ステージモデルのロード ---
    modelStage_ = Model::CreateFromOBJ("stage"); // Resources/Model/stage/stage.obj を想定

    // --- ステージ生成＆初期化 ---
    stage_ = new Stage();

    Vector3 center{ 0.0f, 0.0f, 0.0f }; // 原点に設置
    float startR = 18.0f;                // 初期半径
    float endR   = 3.0f;                 // 最小半径
    float durSec = 60.0f;                // 60秒で縮小

    stage_->Initialize(modelStage_, &camera_, center, startR, endR, durSec);

    // 必要なら任意位置へ移動（center_も更新されます）
    // stage_->SetPosition({ 0.0f, 0.0f, 0.0f });
}

void GameScene::Update() {
    // カメラ操作（固定Δt。実Δtが取れるなら差し替えてOK）
    UpdateCameraControls_(fixedDt_);

    // ステージ更新（半径R(t)の更新＆見た目スケール反映）
    stage_->Update();
}

void GameScene::Draw() {
    ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

    // 背景スプライト
    Sprite::PreDraw(commandList);
    Sprite::PostDraw();
    dxCommon_->ClearDepthBuffer();

    // 3D
    Model::PreDraw(commandList);
    stage_->Draw();
    Model::PostDraw();

    // 前景スプライト
    Sprite::PreDraw(commandList);
    // UI等があればここに
    Sprite::PostDraw();
}

// --- カメラ操作本体（WASD/QE + 右ドラッグで回転） ---
void GameScene::UpdateCameraControls_(float dt) {
	float speed = camSpeed_;
	if (input_->PushKey(DIK_LSHIFT) || input_->PushKey(DIK_RSHIFT)) {
		speed *= camBoost_;
	}

	// Yawから前後左右ベクトルを直接算出
	float cy = std::cos(camYaw_);
	float sy = std::sin(camYaw_);
	Vector3 fwd{sy, 0.0f, cy};
	Vector3 right{cy, 0.0f, -sy};

	Vector3 move{0, 0, 0};
	if (input_->PushKey(DIK_W))
		move += fwd;
	if (input_->PushKey(DIK_S))
		move -= fwd;
	if (input_->PushKey(DIK_D))
		move += right;
	if (input_->PushKey(DIK_A))
		move -= right;
	if (input_->PushKey(DIK_E))
		move.y += 1.0f;
	if (input_->PushKey(DIK_Q))
		move.y -= 1.0f;

	move = Normalize(move);
	camera_.translation_ += move * (speed * dt);

	// マウス右ドラッグで視点回転
	SHORT rmb = GetAsyncKeyState(VK_RBUTTON);
	bool rmbNow = (rmb & 0x8000) != 0;

	POINT cur;
	GetCursorPos(&cur);

	if (rmbNow) {
		if (!rmbHeld_) {
			rmbHeld_ = true;
			prevMousePos_ = cur;
		} else {
			LONG dx = cur.x - prevMousePos_.x;
			LONG dy = cur.y - prevMousePos_.y;
			prevMousePos_ = cur;

			camYaw_ += static_cast<float>(dx) * mouseSens_;
			camPitch_ += static_cast<float>(dy) * mouseSens_;

			const float kMinPitch = -1.55f;
			const float kMaxPitch = 1.55f;
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