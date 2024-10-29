#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Game/3D/Base/BaseGameObject.h"
#include "Game/3D/Base/Collider.h"

/*////////////////////////////////////////////////////////////////////////////////
*								Enemy Class
////////////////////////////////////////////////////////////////////////////////*/
class Enemy
	:public BaseGameObject, public Collider {
public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	Enemy() = default;
	~Enemy() = default;

	void Init();

	void Update(const Matrix4x4& viewPro);

	void Draw();

	//* collision *//

	void OnCollisionEnter(Collider* collider) override;

	// Getter
	bool IsAlive() const;

private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

	bool isAlive_;                   //* 生存フラグ

};