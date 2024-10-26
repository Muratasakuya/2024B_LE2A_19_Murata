#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Game/3D/Base/BaseGameObject.h"

/*////////////////////////////////////////////////////////////////////////////////
*								Player Class
////////////////////////////////////////////////////////////////////////////////*/
class PlayerBullet
	: public BaseGameObject {
public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	PlayerBullet() = default;
	~PlayerBullet() = default;

	void Init(const Vector3& pos, const Vector3& velocity, const Vector3& direction);

	void Update(const Matrix4x4& viewPro);

	void Draw();

	// Getter
	bool IsAlive();

private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

	Vector3 velocity_; //* 速度

	const uint32_t deathTime_ = 180; //* 生存時間
	int32_t deathTimer_;             //* 生存管理
	bool isAlive_;                   //* 生存フラグ

};