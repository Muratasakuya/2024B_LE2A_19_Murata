#include "Particle.h"

#include "Engine/Base/NewMoonGame.h"

/*////////////////////////////////////////////////////////////////////////////////
*							Particle classMethods
////////////////////////////////////////////////////////////////////////////////*/

void Particle::Init() {

	emitter_.count = 3;            //* 発生個数
	emitter_.frequency = 0.5f;     //* 発生頻度

	emitter_.color.SetInit(1.0f);  //* 色

	system_ = std::make_unique<ParticleSystem>();
	system_->Init();

	system_->CreateParticleGroup(emitter_.transform.translate, "uvChecker", "uvChecker", emitter_.count);

}

void Particle::Update() {

	// deltaTime++
	emitter_.frequencyTime += NewMoonGame::GetDeltaTime();

	if (emitter_.frequency <= emitter_.frequencyTime) {

		system_->Emit("uvChecker", emitter_.transform.translate, emitter_.count);

		emitter_.frequencyTime -= emitter_.frequency;
	}

	system_->Update();

}

void Particle::Draw(BlendMode blendMode) {

	system_->Draw("uvChecker", "uvChecker", blendMode);
}
