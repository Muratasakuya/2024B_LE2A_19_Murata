#include "Particle.h"

#include "Engine/Base/NewMoonGame.h"
#include "Engine/Managers/ImGuiManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*							Particle classMethods
////////////////////////////////////////////////////////////////////////////////*/

void Particle::Init() {

	emitter_.count = 4;            //* 発生個数
	emitter_.frequency = 0.1f;     //* 発生頻度

	emitter_.color.SetInit(1.0f);  //* 色

	particleSystem_ = std::make_unique<ParticleSystem>();

	particleSystem_->CreateChaseParticle("teapot.obj", "cube",
		emitter_.transform.translate,
		emitter_.prePos_,
		emitter_.count);

}

void Particle::Update() {

	emitter_.prePos_ = emitter_.transform.translate;

	ImGui::Begin("Particle");

	ImGui::DragFloat3("particleTranslate", &emitter_.transform.translate.x, 0.01f);

	ImGui::End();

	// deltaTime++
	emitter_.frequencyTime += NewMoonGame::GetDeltaTime();

	if (emitter_.frequency <= emitter_.frequencyTime) {

		particleSystem_->EmitChaseParticle("cube",
			emitter_.transform.translate,
			emitter_.prePos_,
			emitter_.count);

		emitter_.frequencyTime -= emitter_.frequency;
	}

	particleSystem_->Update();

}

void Particle::Draw(BlendMode blendMode) {

	particleSystem_->Draw("cube", blendMode);
}
