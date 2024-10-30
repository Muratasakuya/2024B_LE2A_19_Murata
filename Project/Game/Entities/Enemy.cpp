#include "Enemy.h"

#include "Engine/Base/NewMoonGame.h"

/*////////////////////////////////////////////////////////////////////////////////
*								Enemy classMethods
////////////////////////////////////////////////////////////////////////////////*/

void Enemy::Init(const Vector3& pos) {

	BaseGameObject::Init("sphere.obj");

	//* 座標 *//
	transform_.translation = pos;

	//* スケール *//
	const float kInitScale = 0.25f;
	transform_.scale.SetInit(kInitScale);

	Collider::SetCollisionShapeSphere({ .radius = {1.0f * kInitScale + 0.1f} });

	Collider::type_ = ColliderType::Type_Enemy;
	Collider::targetType_ = ColliderType::Type_PlayerBullet;

	Collider::centerPos_ = transform_.translation;

	isAlive_ = true;

}

void Enemy::Update(const Matrix4x4& viewPro) {

	Collider::centerPos_ = transform_.translation;

	BaseGameObject::Update(viewPro);

}

void Enemy::Draw() {

	BaseGameObject::Draw();

}

void Enemy::OnCollisionEnter(Collider* collider) {

	if (collider->GetType() == ColliderType::Type_PlayerBullet) {

		// enemyManagerClass -> remove_if(delete)
		isAlive_ = false;
	}
}

bool Enemy::IsAlive() const {

	return isAlive_;
}