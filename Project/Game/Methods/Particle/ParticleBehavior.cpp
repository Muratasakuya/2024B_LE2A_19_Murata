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

	// 寿命の進行度合い
	float lifeRatio = 1.0f - (particle.currentTime / particle.lifeTime);

	// イージング処理
	if (particle.easingType.has_value()) {
		particle.easedLifeRatio = EasedValue(particle.easingType.value(), lifeRatio);
	}
	float easedLifeRatio = particle.easedLifeRatio.value_or(lifeRatio);

	Vector3 easedVelocity = particle.velocity * easedLifeRatio;
	particle.transform.translate += {
		easedVelocity.x* NewMoonGame::GetDeltaTime(),
			easedVelocity.y* NewMoonGame::GetDeltaTime(),
			easedVelocity.z* NewMoonGame::GetDeltaTime()
	};

	Vector3 scaledTransform = particle.transform.scale * particle.easedLifeRatio.value_or(lifeRatio);
	Matrix4x4 scaleMatrix =
		Matrix4x4::MakeScaleMatrix(scaledTransform);

	particle.color.w = particle.easedLifeRatio.value_or(lifeRatio);

	Matrix4x4 translateMatrix =
		Matrix4x4::MakeTranslateMatrix(particle.transform.translate);

	particle.worldMatrix = scaleMatrix * billboardMatrix * translateMatrix;
	particle.wvpMatrix = particle.worldMatrix * NewMoonGame::GameCamera()->GetCamera3D()->GetViewProjectionMatrix();

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

	// 寿命の進行度合い
	float lifeRatio = 1.0f - (particle.currentTime / particle.lifeTime);

	// イージング処理
	if (particle.easingType.has_value()) {
		particle.easedLifeRatio = EasedValue(particle.easingType.value(), lifeRatio);
	}
	float easedLifeRatio = particle.easedLifeRatio.value_or(lifeRatio);

	Vector3 easedVelocity = particle.velocity * easedLifeRatio;
	particle.transform.translate += {
		easedVelocity.x* NewMoonGame::GetDeltaTime(),
			easedVelocity.y* NewMoonGame::GetDeltaTime(),
			easedVelocity.z* NewMoonGame::GetDeltaTime()
	};

	Vector3 scaledTransform = particle.transform.scale * particle.easedLifeRatio.value_or(lifeRatio);
	Matrix4x4 scaleMatrix =
		Matrix4x4::MakeScaleMatrix(scaledTransform);

	particle.color.w = particle.easedLifeRatio.value_or(lifeRatio);

	Matrix4x4 translateMatrix =
		Matrix4x4::MakeTranslateMatrix(particle.transform.translate);

	particle.worldMatrix = scaleMatrix * billboardMatrix * translateMatrix;
	particle.wvpMatrix = particle.worldMatrix * NewMoonGame::GameCamera()->GetCamera3D()->GetViewProjectionMatrix();

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

	// 寿命の進行度合い
	float lifeRatio = 1.0f - (particle.currentTime / particle.lifeTime);

	// イージング処理
	if (particle.easingType.has_value()) {
		particle.easedLifeRatio = EasedValue(particle.easingType.value(), lifeRatio);
	}
	float easedLifeRatio = particle.easedLifeRatio.value_or(lifeRatio);

	Vector3 easedVelocity = particle.velocity * easedLifeRatio;
	particle.transform.translate += {
		easedVelocity.x* NewMoonGame::GetDeltaTime(),
			easedVelocity.y* NewMoonGame::GetDeltaTime(),
			easedVelocity.z* NewMoonGame::GetDeltaTime()
	};

	Vector3 scaledTransform = particle.transform.scale * particle.easedLifeRatio.value_or(lifeRatio);
	Matrix4x4 scaleMatrix =
		Matrix4x4::MakeScaleMatrix(scaledTransform);

	particle.color.w = particle.easedLifeRatio.value_or(lifeRatio);

	Matrix4x4 translateMatrix =
		Matrix4x4::MakeTranslateMatrix(particle.transform.translate);

	particle.worldMatrix = scaleMatrix * billboardMatrix * translateMatrix;
	particle.wvpMatrix = particle.worldMatrix * NewMoonGame::GameCamera()->GetCamera3D()->GetViewProjectionMatrix();

}

//================================================================================
// InjectionBehavior
//================================================================================

void InjectionBehavior::Create(std::list<ParticleData>& particles, ParticleParameter& parameter) {

	InjectionVisitor visitor;
	parameter.Accept(visitor, particles);
}

void InjectionBehavior::Update(ParticleData& particle, const Matrix4x4& billboardMatrix) {

	particle.currentTime += NewMoonGame::GetDeltaTime();

	// 寿命の進行度合い
	float lifeRatio = 1.0f - (particle.currentTime / particle.lifeTime);

	// イージング処理
	if (particle.easingType.has_value()) {
		particle.easedLifeRatio = EasedValue(particle.easingType.value(), lifeRatio);
	}
	float easedLifeRatio = particle.easedLifeRatio.value_or(lifeRatio);
	Vector3 easedVelocity = particle.velocity * easedLifeRatio;

	if (particle.physics.reflectEnable) {

		// 移動処理前の位置
		Vector3 newPosition = particle.transform.translate + easedVelocity * NewMoonGame::GetDeltaTime();

		// 衝突判定
		if (newPosition.y <= particle.physics.reflectFace.y && particle.velocity.y < 0) {

			particle.velocity = Vector3::Reflect(particle.velocity, Direction::Up()) * particle.physics.restitution;

			// 位置の調整
			newPosition.y = particle.physics.reflectFace.y;
		}

		// 更新された位置に適用
		particle.transform.translate = newPosition;
	} else {

		particle.transform.translate += {
			easedVelocity.x* NewMoonGame::GetDeltaTime(),
				easedVelocity.y* NewMoonGame::GetDeltaTime(),
				easedVelocity.z* NewMoonGame::GetDeltaTime()
		};
	}

	Vector3 gravityEffect =
		particle.physics.gravityDirection.value() * particle.physics.gravityStrength.value() * NewMoonGame::GetDeltaTime();
	particle.velocity += gravityEffect;

	Vector3 scaledTransform = particle.transform.scale * particle.easedLifeRatio.value_or(lifeRatio);
	Matrix4x4 scaleMatrix =
		Matrix4x4::MakeScaleMatrix(scaledTransform);

	particle.color.w = particle.easedLifeRatio.value_or(lifeRatio);

	Matrix4x4 translateMatrix =
		Matrix4x4::MakeTranslateMatrix(particle.transform.translate);

	particle.worldMatrix = scaleMatrix * billboardMatrix * translateMatrix;
	particle.wvpMatrix = particle.worldMatrix * NewMoonGame::GameCamera()->GetCamera3D()->GetViewProjectionMatrix();

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
	case ParticleType::kInjection:

		return std::make_unique<InjectionBehavior>();
	}

	throw std::runtime_error("unKnown particleType");
}