#include "ParticleBehavior.h"

#include "Engine/Base/NewMoonGame.h"
#include "ParticleVisitor.h"
#include "Lib/Adapter/Random.h"

/*////////////////////////////////////////////////////////////////////////////////
*						ParticleBehavior classMethods
////////////////////////////////////////////////////////////////////////////////*/

//================================================================================
// DispersionBehavior
//================================================================================

void DispersionBehavior::Create(std::list<ParticleData>& particles, ParticleParameter& parameter) {

	DispersionVisitor visitor;
	parameter.Accept(visitor, particles);
}

void DispersionBehavior::Update(ParticleData& particle, const Matrix4x4& billboardMatrix) {

	particle.currentTime += NewMoonGame::GetDeltaTime();

	particle.transform.translate += {
		particle.velocity.x* NewMoonGame::GetDeltaTime(),
			particle.velocity.y* NewMoonGame::GetDeltaTime(),
			particle.velocity.z* NewMoonGame::GetDeltaTime()
	};

	float lifeRatio = 1.0f - (particle.currentTime / particle.lifeTime);
	if (particle.easingType.has_value()) {
		particle.easedLifeRatio = EasedValue(particle.easingType.value(), lifeRatio);
	}

	Vector3 scaledTransform = particle.transform.scale * particle.easedLifeRatio.value_or(lifeRatio);
	Matrix4x4 scaleMatrix =
		Matrix4x4::MakeScaleMatrix(scaledTransform);

	Matrix4x4 translateMatrix =
		Matrix4x4::MakeTranslateMatrix(particle.transform.translate);

	particle.worldMatrix = scaleMatrix * billboardMatrix * translateMatrix;
	particle.wvpMatrix = particle.worldMatrix * NewMoonGame::GameCamera()->GetCamera3D()->GetViewProjectionMatrix();

	particle.color.w = particle.easedLifeRatio.value_or(lifeRatio);;

}

//================================================================================
// ChaseBehavior
//================================================================================

void ChaseBehavior::Create(std::list<ParticleData>& particles, ParticleParameter& parameter) {

	ChaseVisitor visitor;
	parameter.Accept(visitor, particles);
}

void ChaseBehavior::Update(ParticleData& particle, const Matrix4x4& billboardMatrix) {

	particle.currentTime += NewMoonGame::GetDeltaTime();

	particle.transform.translate += {
		particle.velocity.x* NewMoonGame::GetDeltaTime(),
			particle.velocity.y* NewMoonGame::GetDeltaTime(),
			particle.velocity.z* NewMoonGame::GetDeltaTime()
	};

	float lifeRatio = 1.0f - (particle.currentTime / particle.lifeTime);
	if (particle.easingType.has_value()) {
		particle.easedLifeRatio = EasedValue(particle.easingType.value(), lifeRatio);
	}

	Vector3 scaledTransform = particle.transform.scale * particle.easedLifeRatio.value_or(lifeRatio);;
	Matrix4x4 scaleMatrix =
		Matrix4x4::MakeScaleMatrix(scaledTransform);

	Matrix4x4 translateMatrix =
		Matrix4x4::MakeTranslateMatrix(particle.transform.translate);

	particle.worldMatrix = scaleMatrix * billboardMatrix * translateMatrix;
	particle.wvpMatrix = particle.worldMatrix * NewMoonGame::GameCamera()->GetCamera3D()->GetViewProjectionMatrix();

	particle.color.w = particle.easedLifeRatio.value_or(lifeRatio);;
}

//================================================================================
// ConvergeBehavior
//================================================================================

void ConvergeBehavior::Create(std::list<ParticleData>& particles, ParticleParameter& parameter) {

	ConvergeVisitor visitor;
	parameter.Accept(visitor, particles);
}

void ConvergeBehavior::Update(ParticleData& particle, const Matrix4x4& billboardMatrix) {

	particle.currentTime += NewMoonGame::GetDeltaTime();

	particle.transform.translate += {
		particle.velocity.x* NewMoonGame::GetDeltaTime(),
			particle.velocity.y* NewMoonGame::GetDeltaTime(),
			particle.velocity.z* NewMoonGame::GetDeltaTime()
	};

	float lifeRatio = 1.0f - (particle.currentTime / particle.lifeTime);
	if (particle.easingType.has_value()) {
		particle.easedLifeRatio = EasedValue(particle.easingType.value(), lifeRatio);
	}

	Vector3 scaledTransform = particle.transform.scale * particle.easedLifeRatio.value_or(lifeRatio);;
	Matrix4x4 scaleMatrix =
		Matrix4x4::MakeScaleMatrix(scaledTransform);

	Matrix4x4 translateMatrix =
		Matrix4x4::MakeTranslateMatrix(particle.transform.translate);

	particle.worldMatrix = scaleMatrix * billboardMatrix * translateMatrix;
	particle.wvpMatrix = particle.worldMatrix * NewMoonGame::GameCamera()->GetCamera3D()->GetViewProjectionMatrix();

	particle.color.w = particle.easedLifeRatio.value_or(lifeRatio);;
}

//================================================================================
// ParticleBehaviorFactory
//================================================================================

std::unique_ptr<ParticleBehavior> ParticleBehaviorFactory::CreateBehavior(ParticleType particleType) {

	switch (particleType) {
	case ParticleType::kDispersion:

		return std::make_unique<DispersionBehavior>();
	case ParticleType::kChase:

		return std::make_unique<ChaseBehavior>();
	case ParticleType::kConverge:

		return std::make_unique<ConvergeBehavior>();
	}

	throw std::runtime_error("unKnown particleType");
}