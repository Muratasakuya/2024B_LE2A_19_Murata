#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Game/3D/Base/BaseGameObject.h"

//* bullet
#include "Game/Enetities/PlayerBullet.h"
#include "Game/2D/Sprite.h"

// c++
#include <list>

/*////////////////////////////////////////////////////////////////////////////////
*								Player Class
////////////////////////////////////////////////////////////////////////////////*/
class Player :
	public BaseGameObject {
public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	Player() = default;
	~Player() = default;

	void Init();

	void Update(const Matrix4x4& viewPro);

	void Draw();

	// Setter
	void SetForward(const Vector3& forward);

private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

	//===================================================================*/
	///* variables

	Vector3 forward_; //* 前方ベクトル

	//* bullet *//

	std::list<std::unique_ptr<PlayerBullet>> bullets_; //* 弾

	std::unique_ptr<Sprite> bulletReticle_;            // 弾の照準
	WorldTransform bulletReticleTransform_;            // 3D照準座標

	//===================================================================*/
	///* functions

	void Shoot();                                       //* 攻撃
	void UpdateBullet(const Matrix4x4& viewPro);        //* 弾更新
	void UpdateBulletReticle(const Matrix4x4& viewPro); //* 弾の照準更新

};