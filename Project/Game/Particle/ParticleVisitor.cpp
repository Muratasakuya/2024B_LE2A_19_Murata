#include "ParticleVisitor.h"

#include "Lib/Adapter/Random.h"

//* front
#include "Game/Particle/ParticleParameter.h"

/*////////////////////////////////////////////////////////////////////////////////
*						ParticleVisitor classMethods
////////////////////////////////////////////////////////////////////////////////*/

Vector4 ParticleVisitor::SettingColor(
	const std::optional<Vector4>& color, const std::optional<RandomParticleColor>& randomParticleColor){

	Vector4 outputColor {};

	if (color.has_value()){

		outputColor = color.value();
	} else{
		if (randomParticleColor.has_value()){

			RandomParticleColor randomColor = randomParticleColor.value();
			switch (randomColor){
				case RandomParticleColor::RED:
					outputColor = Vector4(
						Random::Generate(0.75f, 1.0f), // 赤成分を強く
						Random::Generate(0.0f, 0.2f), // 緑成分を弱め
						Random::Generate(0.0f, 0.2f), // 青成分を弱め
						1.0f);
					break;
				case RandomParticleColor::GREEN:
					outputColor = Vector4(
						Random::Generate(0.0f, 0.2f), // 赤成分を弱め
						Random::Generate(0.75f, 1.0f), // 緑成分を強く
						Random::Generate(0.0f, 0.2f), // 青成分を弱め
						1.0f);
					break;
				case RandomParticleColor::BLUE:
					outputColor = Vector4(
						Random::Generate(0.0f, 0.2f), // 赤成分を弱め
						Random::Generate(0.0f, 0.2f), // 緑成分を弱め
						Random::Generate(0.75f, 1.0f), // 青成分を強く
						1.0f);
					break;
				case RandomParticleColor::PURPLE:
					outputColor = Vector4(
						Random::Generate(0.75f, 1.0f), // 赤成分を強く
						Random::Generate(0.0f, 0.2f), // 緑成分を弱め
						Random::Generate(0.75f, 1.0f), // 青成分を強く
						1.0f);
					break;
				case RandomParticleColor::GRAY:
				{
					float grayValue = Random::Generate(0.3f, 0.7f); // 灰色は赤緑青が均等
					outputColor = Vector4(
						grayValue,
						grayValue,
						grayValue,
						1.0f);
					break;
				}
				case RandomParticleColor::DARKBLUE:
					outputColor = Vector4(
						Random::Generate(0.0f, 0.1f),  // 赤成分をほぼ暗く
						Random::Generate(0.0f, 0.1f),  // 緑成分もほぼ暗く
						Random::Generate(0.4f, 0.8f),  // 青成分を中～高い範囲に設定
						1.0f);
					break;
			}
		} else{

			outputColor = Vector4(
				Random::Generate(0.0f, 1.0f),
				Random::Generate(0.0f, 1.0f),
				Random::Generate(0.0f, 1.0f),
				1.0f);
		}
	}

	return outputColor;
}

//================================================================================
// DispersionVisitor
//================================================================================

void DispersionVisitor::Visit(std::list<ParticleData>& particles, ParticleParameter& parameter){

	if (parameter.isUniform){

		CreateUniformParticles(particles, parameter);
	} else{

		CreateNonUniformParticles(particles, parameter);
	}
}

void DispersionVisitor::CreateUniformParticles(
	std::list<ParticleData>& particles, ParticleParameter& parameter){

	const auto& dispersionParameter = static_cast< DispersionParticleParameter& >(parameter);

	//* FibonacciSphereMethod *//

	const float goldenRatio = (1.0f + std::sqrtf(5.0f)) / 2.0f;

	for (size_t index = 0; index < dispersionParameter.count; ++index){

		ParticleData particle {};
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

		particle.color = SettingColor(dispersionParameter.color, dispersionParameter.randomParticleColor_);

		particle.easingType = parameter.easingType;

		particles.push_back(particle);
	}
}

void DispersionVisitor::CreateNonUniformParticles(
	std::list<ParticleData>& particles, ParticleParameter& parameter){

	const auto& dispersionParameter = static_cast< DispersionParticleParameter& >(parameter);

	for (size_t index = 0; index < dispersionParameter.count; ++index){

		ParticleData particle {};

		particle.transform.rotate.SetInit(0.0f);
		particle.currentTime = 0.0f;

		particle.transform.translate = dispersionParameter.translate;

		particle.transform.scale = Vector3(
			Random::Generate(0.2f, dispersionParameter.scale.x),
			Random::Generate(0.2f, dispersionParameter.scale.y),
			Random::Generate(0.2f, dispersionParameter.scale.z));

		if (dispersionParameter.speed.has_value()){
			particle.velocity = Vector3(
				Random::Generate(-dispersionParameter.speed.value(), dispersionParameter.speed.value()),
				Random::Generate(-dispersionParameter.speed.value(), dispersionParameter.speed.value()),
				Random::Generate(-dispersionParameter.speed.value(), dispersionParameter.speed.value()));
		} else{
			particle.velocity = Vector3(
				Random::Generate(-1.0f, 1.0f),
				Random::Generate(-1.0f, 1.0f),
				Random::Generate(-1.0f, 1.0f));
		}

		particle.color = SettingColor(dispersionParameter.color, dispersionParameter.randomParticleColor_);

		particle.lifeTime = dispersionParameter.lifeTime.value_or(Random::Generate(1.0f, 3.0f));

		particle.easingType = parameter.easingType;

		particles.push_back(particle);
	}
}

//================================================================================
// ChaseVisitor
//================================================================================
void ChaseVisitor::Visit(std::list<ParticleData>& particles, ParticleParameter& parameter){

	if (parameter.isUniform){

		CreateUniformParticles(particles, parameter);
	} else{

		CreateNonUniformParticles(particles, parameter);
	}
}

void ChaseVisitor::CreateUniformParticles(std::list<ParticleData>& particles, ParticleParameter& parameter){

	const ChaseParticleParameter& chaseParticle = static_cast< ChaseParticleParameter& >(parameter);

	for (size_t index = 0; index < chaseParticle.count; ++index){

		ParticleData particle {};

		particle.currentTime = 0.0f;

		particle.transform.translate = chaseParticle.translate;
		particle.transform.scale = chaseParticle.scale;

		Vector3 direction = Vector3::Normalize(chaseParticle.translate - chaseParticle.prePos);
		particle.velocity = chaseParticle.speed.value_or(0.5f) * direction;

		float angle = std::atan2(direction.y, direction.x);
		particle.transform.rotate.SetInit(angle);

		particle.color = SettingColor(chaseParticle.color, chaseParticle.randomParticleColor_);

		particle.lifeTime = chaseParticle.lifeTime.value_or(Random::Generate(1.0f, 2.0f));

		if (chaseParticle.translate == chaseParticle.prePos){

			particle.lifeTime = 0.0f;
		}

		particle.easingType = parameter.easingType;

		particles.push_back(particle);
	}
}

void ChaseVisitor::CreateNonUniformParticles(std::list<ParticleData>& particles, ParticleParameter& parameter){

	const ChaseParticleParameter& chaseParticle = static_cast< ChaseParticleParameter& >(parameter);

	for (size_t index = 0; index < chaseParticle.count; ++index){

		ParticleData particle {};

		particle.transform.rotate.SetInit(0.0f);
		particle.currentTime = 0.0f;

		particle.transform.translate = chaseParticle.translate;
		particle.transform.scale = chaseParticle.scale;

		Vector3 direction = Vector3::Normalize(chaseParticle.translate - chaseParticle.prePos);

		Vector3 randomOffset {};
		randomOffset.SetInit(Random::Generate(-0.4f, 0.4f));
		randomOffset.z = 0.0f;
		Vector3 spreadDirection = Vector3::Normalize(direction + randomOffset);

		if (chaseParticle.speed.has_value()){
			particle.velocity = chaseParticle.speed.value() * spreadDirection;
		} else{
			particle.velocity = Random::Generate(0.4f, 0.8f) * spreadDirection;
		}

		particle.color = SettingColor(chaseParticle.color, chaseParticle.randomParticleColor_);

		particle.lifeTime = chaseParticle.lifeTime.value_or(Random::Generate(1.0f, 2.0f));

		if (chaseParticle.translate == chaseParticle.prePos){

			particle.lifeTime = 0.0f;
		}

		particle.easingType = parameter.easingType;

		particles.push_back(particle);
	}
}

//================================================================================
// ConvergeVisitor
//================================================================================
void ConvergeVisitor::Visit(std::list<ParticleData>& particles, ParticleParameter& parameter){

	if (parameter.isUniform){

		CreateUniformParticles(particles, parameter);
	} else{

		CreateNonUniformParticles(particles, parameter);
	}
}

void ConvergeVisitor::CreateUniformParticles(std::list<ParticleData>& particles, ParticleParameter& parameter){

	const auto& convergeParameter = static_cast< ConvergeParticleParameter& >(parameter);

	//* FibonacciSphereMethod *//

	const float goldenRatio = (1.0f + std::sqrtf(5.0f)) / 2.0f;

	for (size_t index = 0; index < convergeParameter.count; ++index){

		ParticleData particle {};
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

		particle.color = SettingColor(convergeParameter.color, convergeParameter.randomParticleColor_);

		particle.easingType = parameter.easingType;

		particles.push_back(particle);
	}
}

void ConvergeVisitor::CreateNonUniformParticles(std::list<ParticleData>& particles, ParticleParameter& parameter){

	const auto& convergeParameter = static_cast< ConvergeParticleParameter& >(parameter);

	for (size_t index = 0; index < convergeParameter.count; ++index){

		ParticleData particle {};
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

//================================================================================
// InjectionVisitor
//================================================================================
void InjectionVisitor::Visit(std::list<ParticleData>& particles, ParticleParameter& parameter){

	if (parameter.isUniform){

		CreateUniformParticles(particles, parameter);
	} else{

		CreateNonUniformParticles(particles, parameter);
	}
}

void InjectionVisitor::CreateUniformParticles(std::list<ParticleData>& particles, ParticleParameter& parameter){

	const auto& injectionParameter = static_cast< InjectionParticleParameter& >(parameter);

	for (size_t index = 0; index < injectionParameter.count; ++index){

		ParticleData particle {};

		particle.currentTime = 0.0f;

		Vector3 randomPos = Vector3(
			Random::Generate(-1.0f, 1.0f),
			Random::Generate(-1.0f, 1.0f),
			Random::Generate(-1.0f, 1.0f));
		particle.transform.translate = injectionParameter.translate + randomPos;
		
		float randomScale = Random::Generate(injectionParameter.scale.x - 0.05f, injectionParameter.scale.x);
		particle.transform.scale.SetInit(randomScale);


		Vector3 randomOffset = Vector3(
			Random::Generate(-1.0f, 1.0f),
			Random::Generate(-1.0f, 1.0f),
			Random::Generate(-1.0f, 1.0f));
		float randomnessStrength = 0.5f;

		Vector3 finalDirection = injectionParameter.injectionDirection + (randomOffset * randomnessStrength);
		finalDirection.Normalize(finalDirection);

		particle.velocity = injectionParameter.speed.value_or(0.5f) * finalDirection;

		particle.color = SettingColor(injectionParameter.color, injectionParameter.randomParticleColor_);

		particle.physics.gravityDirection = injectionParameter.physics.gravityDirection.value_or(Vector3(0.0f, -1.0f, 0.0f));
		particle.physics.gravityStrength = injectionParameter.physics.gravityStrength.value_or(9.8f);

		particle.lifeTime = injectionParameter.lifeTime.value_or(Random::Generate(2.0f, 8.0f));
		particle.easingType = parameter.easingType;

		particles.push_back(particle);
	}

}

void InjectionVisitor::CreateNonUniformParticles(std::list<ParticleData>& particles, ParticleParameter& parameter){

	const auto& injectionParameter = static_cast< InjectionParticleParameter& >(parameter);

	for (size_t index = 0; index < injectionParameter.count; ++index){

		ParticleData particle {};

		particle.currentTime = 0.0f;

		Vector3 randomPos = Vector3(
			Random::Generate(-1.0f, 1.0f),
			Random::Generate(-1.0f, 1.0f),
			Random::Generate(-1.0f, 1.0f));
		particle.transform.translate = injectionParameter.translate + randomPos;
		float randomScale = Random::Generate(injectionParameter.scale.x - 0.05f, injectionParameter.scale.x);
		particle.transform.scale.SetInit(randomScale);

		Vector3 randomOffset = Vector3(
			Random::Generate(-1.0f, 1.0f),
			Random::Generate(-1.0f, 1.0f),
			Random::Generate(-1.0f, 1.0f));
		float randomnessStrength = 0.5f;

		Vector3 finalDirection = injectionParameter.injectionDirection + (randomOffset * randomnessStrength);
		finalDirection.Normalize(finalDirection);

		particle.velocity = injectionParameter.speed.value_or(0.5f) * finalDirection;

		particle.color = SettingColor(injectionParameter.color, injectionParameter.randomParticleColor_);

		particle.physics.gravityDirection = injectionParameter.physics.gravityDirection.value_or(Vector3(0.0f, -1.0f, 0.0f));
		particle.physics.gravityStrength = injectionParameter.physics.gravityStrength.value_or(9.8f);

		particle.lifeTime = injectionParameter.lifeTime.value_or(Random::Generate(2.0f, 8.0f));
		particle.easingType = parameter.easingType;

		particles.push_back(particle);
	}

}