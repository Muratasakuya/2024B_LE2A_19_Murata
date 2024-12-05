#include "DispersionParticle.h"

#include "Engine/Base/NewMoonGame.h"
#include "Engine/Managers/ImGuiManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*						DispersionParticle classMethods
////////////////////////////////////////////////////////////////////////////////*/

void DispersionParticle::Init() {

	BaseParticle::SetName("dispersionParticle");

	parameter_.scale.min.SetInit(0.25f);
	parameter_.scale.max.SetInit(0.3f);

	parameter_.isUniform = false;

	parameter_.count = 4;        //* 一回で出る個数
	parameter_.frequency = 0.1f; //* 出る間隔
	parameter_.easingType = EasingType::EaseOutCubic;

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