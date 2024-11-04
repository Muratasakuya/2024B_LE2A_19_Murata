#include "TestParticle.h"

#include "Engine/Base/NewMoonGame.h"
#include "Engine/Managers/ImGuiManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*							TestParticle classMethods
////////////////////////////////////////////////////////////////////////////////*/

void TestParticle::Init() {

	parameter_.scale.SetInit(0.25f);

	parameter_.count = 96;
	parameter_.lifeTime = 4.5f;
	parameter_.frequency = parameter_.lifeTime.value() + 1.0f;

	BaseParticle::PresetCreate("cube.obj", ParticleBehaviorType::kDispersion);

}

void TestParticle::Update() {

	// deltaTime++
	parameter_.frequencyTime += NewMoonGame::GetDeltaTime();

	if (parameter_.frequency <= parameter_.frequencyTime) {

		particleSystem_->PresetEmitParticle(
			name_, behaviorType_, parameter_);
		parameter_.frequencyTime -= parameter_.frequency;
	}

	particleSystem_->Update();

}

void TestParticle::DerivedImGui() {
}