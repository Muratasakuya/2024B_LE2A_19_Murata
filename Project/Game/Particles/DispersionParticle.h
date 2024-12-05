#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include <Game/Methods/Particle/BaseParticle.h>

/*////////////////////////////////////////////////////////////////////////////////
*							DispersionParticle Class
////////////////////////////////////////////////////////////////////////////////*/
class DispersionParticle 
	:public BaseParticle<ParticleType::kDispersion> {
public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	DispersionParticle() = default;
	~DispersionParticle() = default;

	void Init() override;

	void Update() override;

	void DerivedImGui() override;

};