#include "DispersionParticle.h"

#include "Engine/Base/NewMoonGame.h"
#include "Engine/Managers/ImGuiManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*						DispersionParticle classMethods
////////////////////////////////////////////////////////////////////////////////*/

void DispersionParticle::Init() {

	BaseParticle::SetName("dispersionParticle");

	parameter_.scale.min.SetInit(0.4f);
	parameter_.scale.max.SetInit(0.5f);

	parameter_.isUniform = false;

	parameter_.count = 2;        //* 一回で出る個数
	parameter_.frequency = 0.1f; //* 出る間隔
	parameter_.easingType = EasingType::EaseOutCubic;

	// EmitterのAABBのサイズと座標
	parameter_.aabb.center = Vector3(0.0f, -1.1f, 0.0f);
	parameter_.aabb.extent = Vector3(1.0f, 1.0f, 1.0f);

	// EmitterのAABBのサイズと座標
	parameter_.aabb.center = Vector3(0.0f, 1.0f, 0.0f);
	parameter_.aabb.extent = Vector3(0.25f, 0.25f, 0.25f);

	BaseParticle::Create("sphere.obj"); //* 使用するModelの名前で初期化

	particleSystem_->SetTexture("dispersionParticle", "circle");

}

void DispersionParticle::Update() {

	BaseParticle::UpdateFrequencyEmit(NewMoonGame::GetDeltaTime());

	particleSystem_->Update();

}

void DispersionParticle::DerivedImGui() {
#ifdef _DEBUG
#endif // _DEBUG
}