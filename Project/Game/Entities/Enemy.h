#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Game/3D/Base/BaseGameObject.h"
#include "Game/3D/Base/Collider.h"

//* 敵の種類
enum class EnemyType {

	kBalloon, // ただその場にふわふわ浮かんでいるだけ // 50点
	kUFO,     // 不規則な動きをする                // 100点
	kGhost,   // 進行方向は一定で不規則に動く        // 150点

	kCount,   // タイプの数
};

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

	void Init(EnemyType type, const Vector3& pos);

	void Update(const Matrix4x4& viewPro);

	void Draw();

	//* collision *//

	void OnCollisionEnter(Collider* collider) override;
	void OnCollisionStay(Collider* collider) override;
	void OnCollisionExit(Collider* collider) override;

	// Getter
	bool IsAlive() const;
	float GetScore() const { return score_; }

private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

	//===================================================================*/
	///* variables

	EnemyType type_;

	float score_;

	bool isAlive_; //* 生存フラグ

	float deathTimer_;

	//* move
	Vector3 targetPosition_;
	Vector3 initPos_;
	Vector3 lerpPos_;
	float lerpProgress_ = 0.0f;

	//===================================================================*/
	///* functions

	void TypeInit();

	void Move();

};