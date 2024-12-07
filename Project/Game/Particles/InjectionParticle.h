#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include <Game/Methods/Particle/BaseParticle.h>

/*////////////////////////////////////////////////////////////////////////////////
*						InjectionParticle Class
////////////////////////////////////////////////////////////////////////////////*/
class InjectionParticle
	:public BaseParticle<ParticleType::kInjection> {
public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	InjectionParticle() = default;
	~InjectionParticle() = default;

	void Init() override;

	void Update() override;

	void DerivedImGui() override;

private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

	//===================================================================*/
	///* variables

	float emitTimer_ = 0.0f; // 0.5秒間隔を管理
	int emitCount_ = 0;      // 4回実行するためのカウンタ

};