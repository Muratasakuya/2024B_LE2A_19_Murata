#include "TestParticle.h"

#include "Engine/Base/NewMoonGame.h"
#include "Engine/Managers/ImGuiManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*							TestParticle classMethods
////////////////////////////////////////////////////////////////////////////////*/

void TestParticle::Init() {

	//* mustSetting *//

	BaseParticle::SetName("testParticle");
	parameter_.scale.min.SetInit(0.15f);
	parameter_.scale.max.SetInit(0.25f);

	//* mainSetting *//

	parameter_.isUniform = false;

	parameter_.count = 4;
	parameter_.frequency = 0.1f;

	//* subSetting *//

	parameter_.sphereScale = 1.0f;

	BaseParticle::Create("cube.obj");

}

void TestParticle::Update() {

	UpdateFrequencyEmit(NewMoonGame::GetDeltaTime());

	particleSystem_->Update();

}

void TestParticle::DerivedImGui() {

	ImGui::DragFloat("sphereScale", &parameter_.sphereScale, 0.01f);

}