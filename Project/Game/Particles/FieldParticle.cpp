#include "FieldParticle.h"

#include "Engine/Base/NewMoonGame.h"
#include "Engine/Managers/ImGuiManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*						FieldParticle classMethods
////////////////////////////////////////////////////////////////////////////////*/

void FieldParticle::Init() {

	BaseParticle::SetName("fieldParticle");

	parameter_.scale.min.SetInit(0.1f);
	parameter_.scale.max.SetInit(0.25f);

	parameter_.isUniform = false;

	parameter_.count = 4;        //* 一回で出る個数
	parameter_.frequency = 0.1f; //* 出る間隔

	// 上に出るようにする
	parameter_.injectionDirection = Vector3(0.0f, 1.0f, 0.0f);
	// 落ちないように重力を0.0fにする
	parameter_.physics.gravityDirection = Vector3::Zero();
	parameter_.physics.gravityStrength = 0.0f;

	parameter_.easingType = EasingType::EaseOutCubic;

	// EmitterのAABBのサイズと座標
	parameter_.aabb.center = Vector3(0.0f, -1.1f, 0.0f);
	parameter_.aabb.extent = Vector3(40.0f, 1.0f, 40.0f);

	BaseParticle::Create("cube.obj"); //* 使用するModelの名前で初期化

	transitionTimer_ = 0.0f;

}

void FieldParticle::Update() {

	UpdateColor();

	BaseParticle::UpdateFrequencyEmit(NewMoonGame::GetDeltaTime());

	particleSystem_->Update();

}

void FieldParticle::DerivedImGui() {
#ifdef _DEBUG

	ImGui::DragFloat3("aabbCenter", &parameter_.aabb.center.x, 0.01f);
	ImGui::DragFloat3("aabbExtent", &parameter_.aabb.extent.x, 0.01f);

#endif // _DEBUG
}

void FieldParticle::UpdateColor() {

	transitionTimer_ += NewMoonGame::GetDeltaTime() / 4.0f;

	// 色を計算 (sin波を使ったゲーミングカラー)
	float r = (std::sin(transitionTimer_) + 1.0f) / 2.0f;        // 0.0～1.0 に正規化
	float g = (std::sin(transitionTimer_ + 2.0f) + 1.0f) / 2.0f; // 2.0の位相ずれ
	float b = (std::sin(transitionTimer_ + 4.0f) + 1.0f) / 2.0f; // 4.0の位相ずれ

	parameter_.color = Vector4(r, g, b, 1.0f);

}