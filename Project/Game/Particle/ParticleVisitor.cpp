#include "ParticleVisitor.h"

#include "Lib/Adapter/Random.h"

//* front
#include "Game/Particle/ParticleParameter.h"

/*////////////////////////////////////////////////////////////////////////////////
*						ParticleVisitor classMethods
////////////////////////////////////////////////////////////////////////////////*/

//================================================================================
// DispersionVisitor
//================================================================================

void DispersionVisitor::Visit(std::list<ParticleData>& particles, ParticleParameter& parameter) {

	if (parameter.isUniform) {

		CreateUniformParticles(particles, parameter);
	} else {

		CreateNonUniformParticles(particles, parameter);
	}
}

void DispersionVisitor::CreateUniformParticles(
	std::list<ParticleData>& particles, ParticleParameter& parameter) {

	const auto& dispersionParameter = static_cast<DispersionParticleParameter&>(parameter);

	//* FibonacciSphereMethod *//

	const float goldenRatio = (1.0f + std::sqrtf(5.0f)) / 2.0f;

	for (size_t index = 0; index < dispersionParameter.count; ++index) {

		ParticleData particle{};
		particle.currentTime = 0.0f;

		float theta = 2.0f * std::numbers::pi_v<float> *(index / goldenRatio);
		float phi = std::acos(1.0f - 2.0f * (index + 0.5f) / dispersionParameter.count);

		float x = std::sin(phi) * std::cos(theta);
		float y = std::sin(phi) * std::sin(theta);
		float z = std::cos(phi);
		Vector3 particlePos = Vector3(x, y, z);

		particle.transform.translate = particlePos * dispersionParameter.sphereScale + dispersionParameter.translate;

		float speedFactor = 1.0f + 0.5f * (1.0f - z);
		particle.velocity = Vector3(x, y, z) * speedFactor * dispersionParameter.speed.value_or(1.0f);

		particle.transform.rotate.SetInit(0.0f);
		particle.transform.scale = dispersionParameter.scale;

		particle.lifeTime = dispersionParameter.lifeTime.value_or(Random::Generate(2.0f, 3.0f));
		particle.color = dispersionParameter.color.value_or(Vector4(
			Random::Generate(0.0f, 1.0f),
			Random::Generate(0.0f, 1.0f),
			Random::Generate(0.0f, 1.0f),
			1.0f));

		particle.easingType = parameter.easingType;

		particles.push_back(particle);
	}
}

void DispersionVisitor::CreateNonUniformParticles(
	std::list<ParticleData>& particles, ParticleParameter& parameter) {

	const auto& dispersionParameter = static_cast<DispersionParticleParameter&>(parameter);

	for (size_t index = 0; index < dispersionParameter.count; ++index) {

		ParticleData particle{};

		particle.transform.rotate.SetInit(0.0f);
		particle.currentTime = 0.0f;

		particle.transform.translate = dispersionParameter.translate;

		particle.transform.scale = Vector3(
			Random::Generate(0.1f, dispersionParameter.scale.x),
			Random::Generate(0.1f, dispersionParameter.scale.y),
			Random::Generate(0.1f, dispersionParameter.scale.z));

		if (dispersionParameter.speed.has_value()) {
			particle.velocity = Vector3(
				Random::Generate(-dispersionParameter.speed.value(), dispersionParameter.speed.value()),
				Random::Generate(-dispersionParameter.speed.value(), dispersionParameter.speed.value()),
				Random::Generate(-dispersionParameter.speed.value(), dispersionParameter.speed.value()));
		} else {
			particle.velocity = Vector3(
				Random::Generate(-1.0f, 1.0f),
				Random::Generate(-1.0f, 1.0f),
				Random::Generate(-1.0f, 1.0f));
		}

		if (dispersionParameter.color.has_value()) {
			particle.color = dispersionParameter.color.value();
		} else {
			particle.color = Vector4(
				Random::Generate(0.0f, 1.0f),
				Random::Generate(0.0f, 1.0f),
				Random::Generate(0.0f, 1.0f),
				1.0f);
		}

		particle.lifeTime = dispersionParameter.lifeTime.value_or(Random::Generate(1.0f, 3.0f));

		particle.easingType = parameter.easingType;

		particles.push_back(particle);
	}
}

//================================================================================
// ChaseVisitor
//================================================================================
void ChaseVisitor::Visit(std::list<ParticleData>& particles, ParticleParameter& parameter) {

	if (parameter.isUniform) {

		CreateUniformParticles(particles, parameter);
	} else {

		CreateNonUniformParticles(particles, parameter);
	}
}

void ChaseVisitor::CreateUniformParticles(std::list<ParticleData>& particles, ParticleParameter& parameter) {

	const ChaseParticleParameter& chaseParticle = static_cast<ChaseParticleParameter&>(parameter);

	for (size_t index = 0; index < chaseParticle.count; ++index) {

		ParticleData particle{};

		particle.currentTime = 0.0f;

		particle.transform.translate = chaseParticle.translate;
		particle.transform.scale = chaseParticle.scale;

		Vector3 direction = Vector3::Normalize(chaseParticle.translate - chaseParticle.prePos);
		particle.velocity = chaseParticle.speed.value_or(0.5f) * direction;

		float angle = std::atan2(direction.y, direction.x);
		particle.transform.rotate.SetInit(angle);

		if (chaseParticle.color.has_value()) {
			particle.color = chaseParticle.color.value();
		} else {
			particle.color = Vector4(
				Random::Generate(0.0f, 1.0f),
				Random::Generate(0.0f, 1.0f),
				Random::Generate(0.0f, 1.0f),
				1.0f);
		}

		particle.lifeTime = chaseParticle.lifeTime.value_or(Random::Generate(1.0f, 2.0f));

		if (chaseParticle.translate == chaseParticle.prePos) {

			particle.lifeTime = 0.0f;
		}

		particle.easingType = parameter.easingType;

		particles.push_back(particle);
	}
}

void ChaseVisitor::CreateNonUniformParticles(std::list<ParticleData>& particles, ParticleParameter& parameter) {

	const ChaseParticleParameter& chaseParticle = static_cast<ChaseParticleParameter&>(parameter);

	for (size_t index = 0; index < chaseParticle.count; ++index) {

		ParticleData particle{};

		particle.transform.rotate.SetInit(0.0f);
		particle.currentTime = 0.0f;

		particle.transform.translate = chaseParticle.translate;
		particle.transform.scale = chaseParticle.scale;

		Vector3 direction = Vector3::Normalize(chaseParticle.translate - chaseParticle.prePos);

		Vector3 randomOffset{};
		randomOffset.SetInit(Random::Generate(-0.4f, 0.4f));
		randomOffset.z = 0.0f;
		Vector3 spreadDirection = Vector3::Normalize(direction + randomOffset);

		if (chaseParticle.speed.has_value()) {
			particle.velocity = chaseParticle.speed.value() * spreadDirection;
		} else {
			particle.velocity = Random::Generate(0.4f, 0.8f) * spreadDirection;
		}

		if (chaseParticle.color.has_value()) {
			particle.color = chaseParticle.color.value();
		} else {
			particle.color = Vector4(
				Random::Generate(0.0f, 1.0f),
				Random::Generate(0.0f, 1.0f),
				Random::Generate(0.0f, 1.0f),
				1.0f);
		}

		particle.lifeTime = chaseParticle.lifeTime.value_or(Random::Generate(1.0f, 2.0f));

		if (chaseParticle.translate == chaseParticle.prePos) {

			particle.lifeTime = 0.0f;
		}

		particle.easingType = parameter.easingType;

		particles.push_back(particle);
	}
}

//================================================================================
// ConvergeVisitor
//================================================================================
void ConvergeVisitor::Visit(std::list<ParticleData>& particles, ParticleParameter& parameter) {

	if (parameter.isUniform) {

		CreateUniformParticles(particles, parameter);
	} else {

		CreateNonUniformParticles(particles, parameter);
	}
}

void ConvergeVisitor::CreateUniformParticles(std::list<ParticleData>& particles, ParticleParameter& parameter) {

	const auto& convergeParameter = static_cast<ConvergeParticleParameter&>(parameter);

	//* FibonacciSphereMethod *//

	const float goldenRatio = (1.0f + std::sqrtf(5.0f)) / 2.0f;

	for (size_t index = 0; index < convergeParameter.count; ++index) {

		ParticleData particle{};
		particle.transform.rotate.SetInit(0.0f);
		particle.currentTime = 0.0f;

		float theta = 2.0f * std::numbers::pi_v<float> *(index / goldenRatio);
		float phi = std::acos(1.0f - 2.0f * (index + 0.5f) / convergeParameter.count);

		float x = std::sin(phi) * std::cos(theta);
		float y = std::sin(phi) * std::sin(theta);
		float z = std::cos(phi);

		Vector3 initialPos = Vector3(x, y, z) * convergeParameter.sphereScale + convergeParameter.translate;
		particle.transform.translate = initialPos;

		Vector3 targetDirection = Vector3::Normalize(convergeParameter.translate - initialPos);
		float speed = convergeParameter.speed.value_or(0.5f);
		particle.velocity = targetDirection * speed;

		particle.transform.scale = convergeParameter.scale;

		float distanceToTarget = Vector3::Length(convergeParameter.translate - initialPos);
		float timeToReachTarget = distanceToTarget / speed;
		particle.lifeTime = convergeParameter.lifeTime.value_or(timeToReachTarget);

		particle.color = convergeParameter.color.value_or(Vector4(
			Random::Generate(0.0f, 1.0f),
			Random::Generate(0.0f, 1.0f),
			Random::Generate(0.0f, 1.0f),
			1.0f));

		particle.easingType = parameter.easingType;

		particles.push_back(particle);
	}
}

void ConvergeVisitor::CreateNonUniformParticles(std::list<ParticleData>& particles, ParticleParameter& parameter) {

	const auto& convergeParameter = static_cast<ConvergeParticleParameter&>(parameter);

	for (size_t index = 0; index < convergeParameter.count; ++index) {

		ParticleData particle{};
		particle.currentTime = 0.0f;
		particle.transform.rotate.SetInit(0.0f);

		Vector3 randomOffset(
			Random::Generate(-convergeParameter.sphereScale, convergeParameter.sphereScale),
			Random::Generate(-convergeParameter.sphereScale, convergeParameter.sphereScale),
			Random::Generate(-convergeParameter.sphereScale, convergeParameter.sphereScale));

		particle.transform.translate = randomOffset + convergeParameter.translate;

		Vector3 targetDirection = Vector3::Normalize(convergeParameter.translate - particle.transform.translate);
		float speed = convergeParameter.speed.value_or(Random::Generate(0.3f, 0.7f));
		particle.velocity = targetDirection * speed;

		particle.transform.scale = Vector3(
			Random::Generate(0.1f, convergeParameter.scale.x),
			Random::Generate(0.1f, convergeParameter.scale.y),
			Random::Generate(0.1f, convergeParameter.scale.z));

		particle.color = convergeParameter.color.value_or(Vector4(
			Random::Generate(0.0f, 1.0f),
			Random::Generate(0.0f, 1.0f),
			Random::Generate(0.0f, 1.0f),
			1.0f));

		float distanceToTarget = Vector3::Length(convergeParameter.translate - particle.transform.translate);
		float timeToReachTarget = distanceToTarget / speed;
		particle.lifeTime = convergeParameter.lifeTime.value_or(timeToReachTarget);

		particle.easingType = parameter.easingType;

		particles.push_back(particle);
	}
}