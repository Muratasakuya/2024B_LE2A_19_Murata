#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Lib/Structure.h"

//* system
#include "Game/Particle/ParticleSystem.h"

// c++
#include <memory>

/*////////////////////////////////////////////////////////////////////////////////
*							BaseParticle Class
////////////////////////////////////////////////////////////////////////////////*/
class BaseParticle {
public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	BaseParticle() = default;
	virtual ~BaseParticle() = default;

	virtual void Init() = 0;
	virtual void Update() = 0;

	virtual void Create(const std::string& modelName, ParticleBehaviorType behaviorType);
	virtual void PresetCreate(const std::string& modelName, ParticleBehaviorType behaviorType);

	virtual void Draw(BlendMode blendMode = BlendMode::kBlendModeNormal);

	virtual void ImGui();
	virtual void DerivedImGui() {};

protected:
	//===================================================================*/
	//							protected Methods
	//===================================================================*/

	//===================================================================*/
	///* variables

	std::string name_;
	ParticleBehaviorType behaviorType_;

	ParticleParameter parameter_;

	std::unique_ptr<ParticleSystem> particleSystem_;

};