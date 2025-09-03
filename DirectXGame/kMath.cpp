#include "kMath.h"
#include <cassert>
#include <cmath>
#include <numbers>

namespace KamataEngine {

Vector3& operator+=(Vector3& lhv, const Vector3& rhv) {
	lhv.x += rhv.x;
	lhv.y += rhv.y;
	lhv.z += rhv.z;
	return lhv;
}

Vector3& operator*=(Vector3& v, float s) {
	v.x *= s;
	v.y *= s;
	v.z *= s;
	return v;
}

Vector3& operator-=(Vector3& lhs, const Vector3& rhv) {
	lhs.x -= rhv.x;
	lhs.y -= rhv.y;
	lhs.z -= rhv.z;
	return lhs;
}
const Vector3 operator*(const Vector3& v, float s) {
	Vector3 temp(v);
	return temp *= s;
}

const Vector3 operator+(const Vector3& v1, const Vector3& v2) {
	Vector3 temp(v1);
	return temp += v2;
}

const Vector3 operator-(const Vector3& v1, const Vector3& v2) { return Vector3{v1.x - v2.x, v1.y - v2.y, v1.z - v2.z}; }

Matrix4x4 Multiply(const Matrix4x4& matrix1, const Matrix4x4& matrix2) {
	Matrix4x4 result = {};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				result.m[i][j] += matrix1.m[i][k] * matrix2.m[k][j];
			}
		}
	}
	return result;
}

Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 ans = {};
	ans.m[0][0] = scale.x;
	ans.m[1][1] = scale.y;
	ans.m[2][2] = scale.z;
	ans.m[3][3] = 1;
	return ans;
}

Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
	Matrix4x4 ans = {};
	ans.m[0][0] = 1;
	ans.m[1][1] = 1;
	ans.m[2][2] = 1;
	ans.m[3][0] = translate.x;
	ans.m[3][1] = translate.y;
	ans.m[3][2] = translate.z;
	ans.m[3][3] = 1;
	return ans;
}

Matrix4x4 MakeRotateXMatrix(float radian) {
	Matrix4x4 ans = {};
	ans.m[0][0] = 1;
	ans.m[1][1] = cosf(radian);
	ans.m[1][2] = sinf(radian);
	ans.m[2][1] = -sinf(radian);
	ans.m[2][2] = cosf(radian);
	ans.m[3][3] = 1;
	return ans;
}

Matrix4x4 MakeRotateYMatrix(float radian) {
	Matrix4x4 ans = {};
	ans.m[0][0] = cosf(radian);
	ans.m[0][2] = -sinf(radian);
	ans.m[1][1] = 1;
	ans.m[2][0] = sinf(radian);
	ans.m[2][2] = cosf(radian);
	ans.m[3][3] = 1;
	return ans;
}

Matrix4x4 MakeRotateZMatrix(float radian) {
	Matrix4x4 ans = {};
	ans.m[0][0] = cosf(radian);
	ans.m[0][1] = sinf(radian);
	ans.m[1][0] = -sinf(radian);
	ans.m[1][1] = cosf(radian);
	ans.m[2][2] = 1;
	ans.m[3][3] = 1;
	return ans;
}

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translation) {
	Matrix4x4 ans =
	    Multiply(Multiply(MakeScaleMatrix(scale), Multiply(MakeRotateXMatrix(rotate.x), Multiply(MakeRotateYMatrix(rotate.y), MakeRotateZMatrix(rotate.z)))), MakeTranslateMatrix(translation));
	return ans;
}

Matrix4x4 PlayerAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translation) {
	(void)scale;
	Matrix4x4 ansMat = Multiply(MakeRotateYMatrix(rotate.y), MakeTranslateMatrix(translation));
	return ansMat;
}

float EaseInOut(float x1, float x2, float t) {
	float easedT = -(std::cosf(std::numbers::pi_v<float> * t) - 1.0f) / 2.0f;
	return Lerp(x1, x2, easedT);
}

float Lerp(float x1, float x2, float t) { return (1.0f - t) * x1 + t * x2; }

Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) { return Vector3(Lerp(v1.x, v2.x, t), Lerp(v1.y, v2.y, t), Lerp(v1.z, v2.z, t)); }

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 ans;
	ans.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + matrix.m[3][0];
	ans.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + matrix.m[3][1];
	ans.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + matrix.m[3][3];
	assert(w != 0.0f);
	ans.x /= w;
	ans.y /= w;
	ans.z /= w;
	return ans;
}

Vector4 Transform(const Vector4& vector, const Matrix4x4& matrix) {
	Vector4 result;
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + vector.w * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + vector.w * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + vector.w * matrix.m[3][2];
	result.w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + vector.w * matrix.m[3][3];
	return result;
}

Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m) {
	return Vector3{v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0], v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1], v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2]};
}

Vector2 WorldToScreen(const Vector3& worldPos, const Matrix4x4& view, const Matrix4x4& projection, int screenWidth, int screenHeight) {
	Matrix4x4 vp = Multiply(view, projection);
	Vector3 ndc = Transform(worldPos, vp);
	Vector2 screenPos;
	screenPos.x = (ndc.x * 0.5f + 0.5f) * screenWidth;
	screenPos.y = (1.0f - (ndc.y * 0.5f + 0.5f)) * screenHeight;
	return screenPos;
}

Vector3 ScreenToWorld(const Vector2& screenPos, float zDepth, const Matrix4x4& viewMatrix, const Matrix4x4& projectionMatrix, int screenWidth, int screenHeight) {
	Vector3 ndc;
	ndc.x = (screenPos.x / static_cast<float>(screenWidth)) * 2.0f - 1.0f;
	ndc.y = 1.0f - (screenPos.y / static_cast<float>(screenHeight)) * 2.0f;
	ndc.z = zDepth; // ここが重要！

	Matrix4x4 viewProj = Multiply(viewMatrix, projectionMatrix);
	Matrix4x4 invViewProj = KamataEngine::MathUtility::Inverse(viewProj);

	return Transform(ndc, invViewProj);
}

} // namespace KamataEngine
