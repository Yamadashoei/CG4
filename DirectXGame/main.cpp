#include "KamataEngine.h"
#include <Windows.h>

#include "GameScene.h"

using namespace KamataEngine;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// エンジンの初期化
	KamataEngine::Initialize(L"LE3C_27_ヤマダ_ショウエイ");

	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// ゲームシーンの初期化
	GameScene* gameScene = new GameScene();
	gameScene->Initialize();

	// メインループ
	while (true) {
		// エンジンの更新処理
		if (KamataEngine::Update()) {
			break; // 終了リクエストがあればループを抜ける
		}

		// ゲームシーンの更新処理
		gameScene->Update();

		// 描画開始
		dxCommon->PreDraw();

		// ゲームシーンの描画処理
		gameScene->Draw();

		// 描画終了
		dxCommon->PostDraw();
	}

	// ゲームシーンの解放
	delete gameScene;
	gameScene = nullptr;

	// エンジンの終了処理
	KamataEngine::Finalize();

	return 0;
}
