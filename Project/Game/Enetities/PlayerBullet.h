#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Game/3D/Base/BaseGameObject.h"
#include "Game/3D/Base/Collider.h"

/*////////////////////////////////////////////////////////////////////////////////
*								Player Class
////////////////////////////////////////////////////////////////////////////////*/
class PlayerBullet
	: public BaseGameObject, public Collider {
public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	PlayerBullet() = default;
	~PlayerBullet() = default;

	void Init(const Vector3& pos, const Vector3& velocity, const Vector3& direction);

	void Update(const Matrix4x4& viewPro);

	void Draw();

	//* collision *//

	void OnCollisionEnter(Collider* collider) override;

	// Getter
	bool IsAlive() const ;

private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

	Vector3 velocity_; //* 速度

	const uint32_t deathTime_ = 180; //* 生存時間
	int32_t deathTimer_;             //* 生存管理
	bool isAlive_;                   //* 生存フラグ

};