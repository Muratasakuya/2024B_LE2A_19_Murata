#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Lib/Structure.h"

// c++
#include <memory>
#include <vector>

//** Types **//
enum class ParticleBehaviorType {

	kDispersion, // 分散
	kChase,      // 追跡
};

/*////////////////////////////////////////////////////////////////////////////////
*						ParticleBehavior Class
////////////////////////////////////////////////////////////////////////////////*/
class ParticleBehavior {
public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	ParticleBehavior() = default;
	virtual ~ParticleBehavior() = default;

	virtual void Create(ParticleData& particle, const ParticleParameter& parameter) = 0;
	virtual void PresetCreate(std::vector<ParticleData>& particles, const ParticleParameter& parameter) = 0;

	virtual void Update(ParticleData& particle, const Matrix4x4& billboardMatrix) = 0;

	// Getter
	virtual ParticleBehaviorType GetType() const { return type_; }

protected:
	//===================================================================*/
	//							protected Methods
	//===================================================================*/

	ParticleBehaviorType type_;

};

class DispersionBehavior
	: public ParticleBehavior {
public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	DispersionBehavior() { type_ = ParticleBehaviorType::kDispersion; };
	~DispersionBehavior() = default;

	void Create(ParticleData& particle, const ParticleParameter& parameter) override;
	void PresetCreate(std::vector<ParticleData>& particles, const ParticleParameter& parameter) override;

	void Update(ParticleData& particle, const Matrix4x4& billboardMatrix) override;

};

class ChaseBehavior
	: public ParticleBehavior {
public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	ChaseBehavior() { type_ = ParticleBehaviorType::kChase; };
	~ChaseBehavior() = default;

	void Create(ParticleData& particle, const ParticleParameter& parameter) override;
	void PresetCreate(std::vector<ParticleData>& particles, const ParticleParameter& parameter) override;

	void Update(ParticleData& particle, const Matrix4x4& billboardMatrix) override;

};

//================================================================================
// ParticleBehaviorFactory
//================================================================================
class ParticleBehaviorFactory {
public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	ParticleBehaviorFactory() = default;
	~ParticleBehaviorFactory() = default;

	static std::unique_ptr<ParticleBehavior> CreateBehavior(ParticleBehaviorType behaviorType);

};