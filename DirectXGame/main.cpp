#include "KamataEngine.h"
#include <Windows.h>

#include "TitleScene.h"
#include "GameScene.h"

using namespace KamataEngine;

enum class SceneType { Title, Game };

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// エンジンの初期化
	KamataEngine::Initialize(L"LE3C_27_ヤマダ_ショウエイ");
	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();


	SceneType currentScene = SceneType::Title;
	TitleScene* titleScene = new TitleScene();
	GameScene* gameScene = nullptr;

	titleScene->Initialize();


	// メインループ
	while (true) {
		// エンジンの更新
		if (KamataEngine::Update()) {
			break;
		}


		// 更新処理
		switch (currentScene) {
		case SceneType::Title:
			titleScene->Update();
			if (titleScene->IsSceneEnd()) {
				gameScene = new GameScene();
				gameScene->Initialize();
				currentScene = SceneType::Game;
			}
			break;

		case SceneType::Game:
			gameScene->Update();
			break;
		}


		// 描画処理
		dxCommon->PreDraw();

		switch (currentScene) {
		case SceneType::Title:
			titleScene->Draw();
			break;
		case SceneType::Game:
			gameScene->Draw();
			break;
		}
		dxCommon->PostDraw();



	}

// 解放処理
	delete titleScene;
	delete gameScene;


	// 解放処理
	KamataEngine::Finalize();

	return 0;
}
