#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Game/3D/Base/BaseGameObject.h"
#include "Game/3D/Base/Collider.h"

//* particle
#include "Game/Particles/PlayerChaseParticle.h"

// c++
#include <memory>

/*////////////////////////////////////////////////////////////////////////////////
*								Player Class
////////////////////////////////////////////////////////////////////////////////*/
class Player
	:public BaseGameObject {
public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	Player() = default;
	~Player();

	void Init();

	void Update();

	void Draw();

	void DerivedImGui() override;

	//* getter *//

	Vector3 GetForward() const;

private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

	//===================================================================*/
	///* variables

	float moveSpeed_;

	std::unique_ptr<PlayerChaseParticle> chaseParticle_;

	//===================================================================*/
	///* functions

	void KeyMove();

};