#pragma once
#include <KamataEngine.h>
#include <math.h>
#include <math/Matrix4x4.h>
#include <math/Vector2.h>
#include <math/Vector3.h>

namespace KamataEngine {

// 行列の積
Matrix4x4 Multiply(const Matrix4x4& matrix1, const Matrix4x4& matrix2);

// スケール行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale);

// 平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

// 回転行列（X/Y/Z）
Matrix4x4 MakeRotateXMatrix(float radian);
Matrix4x4 MakeRotateYMatrix(float radian);
Matrix4x4 MakeRotateZMatrix(float radian);

// アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translation);
Matrix4x4 PlayerAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translation);

// イージング / 補間
float EaseInOut(float x1, float x2, float t);
float Lerp(float x1, float x2, float t);
Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);

// 演算子オーバーロード
Vector3& operator+=(Vector3& lhs, const Vector3& rhv);
Vector3& operator*=(Vector3& v, float s);
Vector3& operator-=(Vector3& lhs, const Vector3& rhv);

const Vector3 operator*(const Vector3& v, float s);
const Vector3 operator+(const Vector3& v1, const Vector3& v2);
const Vector3 operator-(const Vector3& v1, const Vector3& v2);

// ベクトル変換
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);
Vector4 Transform(const Vector4& vector, const Matrix4x4& matrix);

// ユーティリティ
inline Vector3 Subtract(const Vector3& a, const Vector3& b) { return {a.x - b.x, a.y - b.y, a.z - b.z}; }
inline float Vector3Length(const Vector3& v) { return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z); }
inline Vector3 Normalize(const Vector3& v) {
	float len = Vector3Length(v);
	if (len == 0.0f)
		return {0.0f, 0.0f, 0.0f};
	return {v.x / len, v.y / len, v.z / len};
}

// スクリーン座標変換
Vector2 WorldToScreen(const Vector3& worldPos, const Matrix4x4& view, const Matrix4x4& projection, int screenWidth, int screenHeight);
Vector3 ScreenToWorld(const Vector2& screenPos, float zDepth, const Matrix4x4& viewMatrix, const Matrix4x4& projectionMatrix, int screenWidth, int screenHeight);

} // namespace KamataEngine
