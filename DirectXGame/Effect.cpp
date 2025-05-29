#include "Effect.h"
#include <algorithm>
#include <cassert>
#include <random>

using namespace KamataEngine;
using namespace MathUtility;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<float> velocity(-0.05f, 0.05f);

void Effect::Initialize(Model* model, Vector3 scale, Vector3 rotation, Vector3 position) {
	assert(model);
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.scale_ = scale;
	worldTransform_.rotation_ = rotation;
	worldTransform_.translation_ = position;

	objectColor_.Initialize();
	color_ = {1.0f, 0.5f, 0.0f, 1.0f};
	originalScale_ = scale;

	velocity_ = {velocity(gen), velocity(gen) + 0.05f, velocity(gen)};
}

void Effect::Update() {
	if (isFinished_)
		return;

	count_ += 1.0f / 60.0f;
	if (count_ >= kDuration) {
		count_ = kDuration;
		isFinished_ = true;
	}

	color_.w = 1.0f - (count_ / kDuration);

	float expansion = 1.5f - (count_ / kDuration) * 1.0f;
	worldTransform_.scale_ = originalScale_ * expansion;
	worldTransform_.translation_ = worldTransform_.translation_ + velocity_;

	worldTransform_.UpdateMatrix();
	objectColor_.SetColor(color_);
}

void Effect::Draw(Camera& camera) { model_->Draw(worldTransform_, camera, &objectColor_); }