#pragma once

//===================================================================*/
//	include
//===================================================================*/
#include "Game/3D/Base/BaseGameObject.h"
#include "Game/3D/Base/Collider.h"
#include "Game/Methods/Utility/MoveUtility.h"

// c++
#include <memory>

/*////////////////////////////////////////////////////////////////////////////////
*	Player Class
////////////////////////////////////////////////////////////////////////////////*/
class Player
	:public BaseGameObject,
	public Collider {
public:
	//===================================================================*/
	//	public Methods
	//===================================================================*/

	Player() = default;
	~Player() = default;

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

	MoveUtility move_;

	//===================================================================*/
	///* functions

	//* Move *//

	void Move();

};