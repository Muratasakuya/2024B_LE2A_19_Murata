#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include <Game/Methods/Particle/BaseParticle.h>

/*////////////////////////////////////////////////////////////////////////////////
*						PlayerChaseParticle Class
////////////////////////////////////////////////////////////////////////////////*/
class PlayerChaseParticle
	:public BaseParticle<ParticleType::kChase> {
public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	PlayerChaseParticle() = default;
	~PlayerChaseParticle() = default;

	void Init() override;

	void Update() override;

	//* setter *//

	void SetPos(const Vector3& pos);


};