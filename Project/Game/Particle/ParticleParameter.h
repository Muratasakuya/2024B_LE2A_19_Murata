#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Lib/Structure.h"
#include "Lib/Adapter/Easing.h"

// c++
#include <vector>
#include <optional>

//* front
class ParticleVisitor;

/*////////////////////////////////////////////////////////////////////////////////
*						ParticleParameter Class
////////////////////////////////////////////////////////////////////////////////*/
class ParticleParameter {
public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	ParticleParameter() = default;
	virtual ~ParticleParameter() = default;

	virtual void Accept(ParticleVisitor& visitor, std::list<ParticleData>& particles) = 0;

	bool isUniform; // 均一か非均一

	Vector3 translate;
	Vector3 scale;

	std::optional<float> speed = std::nullopt;
	std::optional<float> lifeTime = std::nullopt;
	std::optional<Vector4> color = std::nullopt;
	std::optional<RandomParticleColor> randomParticleColor_;

	ParticlePhysics physics;

	//* easing *//
	std::optional<EasingType> easingType = std::nullopt;

	//* emitter *//

	uint32_t count;      // 個数
	float frequency;     // ~秒置き、発生頻度
	float frequencyTime; // 発生頻度用の時刻

};

//================================================================================
//							DispersionParticleParameter
//================================================================================
class DispersionParticleParameter
	: public ParticleParameter {
public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	DispersionParticleParameter() = default;
	~DispersionParticleParameter() = default;

	void Accept(ParticleVisitor& visitor, std::list<ParticleData>& particles) override;

	float sphereScale;

};

//================================================================================
//								ChaseParticleParameter
//================================================================================
class ChaseParticleParameter
	: public ParticleParameter {
public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	ChaseParticleParameter() = default;
	~ChaseParticleParameter() = default;

	void Accept(ParticleVisitor& visitor, std::list<ParticleData>& particles) override;

	Vector3 prePos;

};

//================================================================================
//								ConvergeParticleParameter
//================================================================================
class ConvergeParticleParameter
	: public ParticleParameter {
public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	ConvergeParticleParameter() = default;
	~ConvergeParticleParameter() = default;

	void Accept(ParticleVisitor& visitor, std::list<ParticleData>& particles) override;

	float sphereScale;

};

//================================================================================
//								InjectionParticleParameter
//================================================================================
class InjectionParticleParameter
	: public ParticleParameter {
public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	InjectionParticleParameter() = default;
	~InjectionParticleParameter() = default;

	void Accept(ParticleVisitor& visitor, std::list<ParticleData>& particles) override;

	Vector3 injectionDirection;

};