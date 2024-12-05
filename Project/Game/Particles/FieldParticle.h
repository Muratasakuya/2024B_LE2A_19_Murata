#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include <Game/Methods/Particle/BaseParticle.h>

/*////////////////////////////////////////////////////////////////////////////////
*							FieldParticle Class
////////////////////////////////////////////////////////////////////////////////*/
class FieldParticle
	:public BaseParticle<ParticleType::kInjection> {
public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	FieldParticle() = default;
	~FieldParticle() = default;

	void Init() override;

	void Update() override;

	void DerivedImGui() override;

	//* getter *//
	AABBInfo GetAABB() const { return parameter_.aabb; }

private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

	//===================================================================*/
	///* variables

	float transitionTimer_;

	//===================================================================*/
	///* function

	void UpdateColor();

};