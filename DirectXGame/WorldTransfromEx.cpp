#include <KamataEngine.h>

using namespace KamataEngine;
using namespace MathUtility;

void WorldTransform::UpdateMatrix() {
	// スケーリング行列の作成
	Matrix4x4 matScale = MakeScaleMatrix(scale_);
	// 回転行列の作成
	Matrix4x4 matRotX = MakeRotateXMatrix(rotation_.x);
	Matrix4x4 matRotY = MakeRotateYMatrix(rotation_.y);
	Matrix4x4 matRotZ = MakeRotateZMatrix(rotation_.z);
	Matrix4x4 matRotZX = matRotZ * matRotX;
	Matrix4x4 matRot = matRotZX * matRotY;

	// 平行移動行列の作成
	Matrix4x4 matTrans = MakeTranslateMatrix(translation_);
	// スケーリング、回転、平行移動の合成
	matWorld_ = matScale * matRot * matTrans;

	// 定数バッファに転送する
	TransferMatrix();
}