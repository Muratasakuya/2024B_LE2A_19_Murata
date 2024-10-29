#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Lib/Structure.h"

//* system
#include "Game/Methods/ParticleSystem.h"

// c++
#include <memory>

/*////////////////////////////////////////////////////////////////////////////////
*								Particle Class
////////////////////////////////////////////////////////////////////////////////*/
class Particle {
public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	Particle() = default;
	~Particle() = default;

	void Init();

	void Update();

	void Draw(BlendMode blendMode = BlendMode::kBlendModeNormal);

private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

	//===================================================================*/
	///* variables

	Emitter emitter_;

	std::unique_ptr<ParticleSystem> particleSystem_;

};