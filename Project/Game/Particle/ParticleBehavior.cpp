#include "ParticleBehavior.h"

#include "Engine/Base/NewMoonGame.h"
#include "Lib/Adapter/Random.h"

/*////////////////////////////////////////////////////////////////////////////////
*						ParticleBehavior classMethods
////////////////////////////////////////////////////////////////////////////////*/

//================================================================================
// DispersionBehavior
//================================================================================

void DispersionBehavior::Create(ParticleData& particle, const ParticleParameter& parameter) {

	// 固定値
	particle.transform.rotate.SetInit(0.0f);
	particle.currentTime = 0.0f;

	particle.transform.translate = parameter.translate;
	particle.transform.scale = parameter.scale;

	if (parameter.speed.has_value()) {
		particle.velocity = Vector3(
			Random::Generate(-parameter.speed.value(), parameter.speed.value()),
			Random::Generate(-parameter.speed.value(), parameter.speed.value()),
			Random::Generate(-parameter.speed.value(), parameter.speed.value()));
	} else {
		particle.velocity = Vector3(
			Random::Generate(-1.0f, 1.0f),
			Random::Generate(-1.0f, 1.0f),
			Random::Generate(-1.0f, 1.0f));
	}

	if (parameter.color.has_value()) {
		particle.color = parameter.color.value();
	} else {
		particle.color = Vector4(
			Random::Generate(0.0f, 1.0f),
			Random::Generate(0.0f, 1.0f),
			Random::Generate(0.0f, 1.0f),
			1.0f);
	}

	particle.lifeTime = parameter.lifeTime.value_or(Random::Generate(1.0f, 3.0f));
}

void DispersionBehavior::PresetCreate(std::vector<ParticleData>& particles, const ParticleParameter& parameter) {

	particles.resize(parameter.count);

	const float goldenRatio = (1.0f + std::sqrtf(5.0f)) / 2.0f; //* Fibonacci sphere

	for (size_t index = 0; index < parameter.count; ++index) {

		ParticleData& particle = particles[index];
		particle.currentTime = 0.0f;

		// Fibonacci sphere
		float theta = 2.0f * std::numbers::pi_v<float> *(index / goldenRatio);
		float phi = std::acos(1.0f - 2.0f * (index + 0.5f) / parameter.count);

		// 球面座標 -> デカルト座標
		float x = std::sin(phi) * std::cos(theta);
		float y = std::sin(phi) * std::sin(theta);
		float z = std::cos(phi);
		Vector3 particlePos = Vector3(x, y, z);

		particle.transform.translate = particlePos + parameter.translate;

		float speedFactor = 1.0f + 0.5f * (1.0f - z);
		particle.velocity = Vector3(x, y, z) * speedFactor;

		particle.transform.rotate.SetInit(0.0f);
		particle.transform.scale = parameter.scale;

		if (parameter.lifeTime.has_value()) {
			particle.lifeTime =
				Random::Generate(parameter.lifeTime.value() - 1.0f, parameter.lifeTime.value());
		} else {
			particle.lifeTime = Random::Generate(2.0f, 3.0f);
		}

		if (parameter.color.has_value()) {
			particle.color = parameter.color.value();
		} else {
			particle.color = Vector4(
				Random::Generate(0.7f, 1.0f),
				Random::Generate(0.7f, 1.0f),
				Random::Generate(0.7f, 1.0f),
				1.0f);
		}
	}
}

void DispersionBehavior::Update(ParticleData& particle, const Matrix4x4& billboardMatrix) {

	particle.currentTime += NewMoonGame::GetDeltaTime();

	particle.transform.translate += {
		particle.velocity.x* NewMoonGame::GetDeltaTime(),
			particle.velocity.y* NewMoonGame::GetDeltaTime(),
			particle.velocity.z* NewMoonGame::GetDeltaTime()
	};

	float lifeRatio = 1.0f - (particle.currentTime / particle.lifeTime);

	Vector3 scaledTransform = particle.transform.scale * lifeRatio;
	Matrix4x4 scaleMatrix =
		Matrix4x4::MakeScaleMatrix(scaledTransform);

	Matrix4x4 translateMatrix =
		Matrix4x4::MakeTranslateMatrix(particle.transform.translate);

	particle.worldMatrix = scaleMatrix * billboardMatrix * translateMatrix;
	particle.wvpMatrix = particle.worldMatrix * NewMoonGame::GameCamera()->GetCamera3D()->GetViewProjectionMatrix();

	particle.color.w = lifeRatio;

}

//================================================================================
// ChaseBehavior
//================================================================================

void ChaseBehavior::Create(ParticleData& particle, const ParticleParameter& parameter) {

	// 固定値
	particle.transform.scale.SetInit(1.0f);
	particle.transform.rotate.SetInit(0.0f);
	particle.currentTime = 0.0f;
	particle.easedT_ = std::nullopt;

	particle.transform.translate = parameter.translate;
	particle.transform.scale = parameter.scale;

	Vector3 direction = Vector3::Normalize(parameter.translate - parameter.prePos);

	Vector3 randomOffset{};
	randomOffset.SetInit(Random::Generate(-0.4f, 0.4f));
	randomOffset.z = 0.0f;
	Vector3 spreadDirection = Vector3::Normalize(direction + randomOffset);

	if (parameter.speed.has_value()) {
		particle.velocity = parameter.speed.value() * spreadDirection;
	} else {
		particle.velocity = Random::Generate(0.4f, 0.8f) * spreadDirection;
	}

	if (parameter.color.has_value()) {
		particle.color = parameter.color.value();
	} else {
		particle.color = Vector4(
			Random::Generate(0.0f, 1.0f),
			Random::Generate(0.0f, 1.0f),
			Random::Generate(0.0f, 1.0f),
			1.0f);
	}

	particle.lifeTime = parameter.lifeTime.value_or(Random::Generate(1.0f, 2.0f));

	if (parameter.translate == parameter.prePos) {

		particle.lifeTime = 0.0f;
	}

}

void ChaseBehavior::PresetCreate(std::vector<ParticleData>& particles, const ParticleParameter& parameter) {

	//* 未作成 *//

	particles;
	parameter;

}

void ChaseBehavior::Update(ParticleData& particle, const Matrix4x4& billboardMatrix) {

	particle.transform.translate += {
		particle.velocity.x* NewMoonGame::GetDeltaTime(),
			particle.velocity.y* NewMoonGame::GetDeltaTime(),
			particle.velocity.z* NewMoonGame::GetDeltaTime()
	};

	billboardMatrix;
}

//================================================================================
// ParticleBehaviorFactory
//================================================================================

std::unique_ptr<ParticleBehavior> ParticleBehaviorFactory::CreateBehavior(ParticleBehaviorType behaviorType) {

	switch (behaviorType) {
	case ParticleBehaviorType::kDispersion:

		return std::make_unique<DispersionBehavior>();
	case ParticleBehaviorType::kChase:

		return std::make_unique<ChaseBehavior>();
	}

	throw std::runtime_error("unKnown behaviorType");
}