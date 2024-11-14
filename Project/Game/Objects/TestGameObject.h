#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Game/3D/Base/BaseGameObject.h"
#include "Game/3D/Base/Collider.h"

/*////////////////////////////////////////////////////////////////////////////////
*							TestGameObject Class
////////////////////////////////////////////////////////////////////////////////*/
class TestGameObject
	:public BaseGameObject, public Collider {
public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	TestGameObject() = default;
	~TestGameObject();

	void Init();

	void Update();

	void Draw();

	void DerivedImGui() override;

	// * collision

	void OnCollisionEnter([[maybe_unused]] Collider* other);

	void OnCollisionStay([[maybe_unused]] Collider* other);

	void OnCollisionExit([[maybe_unused]] Collider* other);

private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

	//===================================================================*/
	///* variables



	//===================================================================*/
	///* functions

	void KeyMove();

};