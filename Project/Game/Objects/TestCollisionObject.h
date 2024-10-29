#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Game/3D/Base/BaseGameObject.h"
#include "Game/3D/Base/Collider.h"

/*////////////////////////////////////////////////////////////////////////////////
*						TestCollisionObject Class
////////////////////////////////////////////////////////////////////////////////*/
class TestCollisionObject
	:public BaseGameObject, public Collider {
public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	TestCollisionObject() = default;
	~TestCollisionObject() = default;

	void Init();

	void Update(const Matrix4x4& viewPro);

	void Draw();

	//* collision *//

	void OnCollisionEnter(Collider* collider) override;

	void OnCollisionStay(Collider* collider) override;

	void OnCollisionExit(Collider* collider) override;

public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

};