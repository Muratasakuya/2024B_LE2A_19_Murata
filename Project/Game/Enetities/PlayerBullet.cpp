#include "PlayerBullet.h"

#include "Engine/Base/NewMoonGame.h"
#include "Engine/Managers/ImGuiManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*								PlayerBullet classMethods
////////////////////////////////////////////////////////////////////////////////*/

void PlayerBullet::Init(const Vector3& pos, const Vector3& velocity, const Vector3& direction) {

	BaseGameObject::Init("sphere.obj");

	velocity_ = velocity;
	transform_.translation = pos;

	//* スケール *//
	const float kInitScale = 0.1f;
	transform_.scale.SetInit(kInitScale);

	//* 回転の計算 *//
	transform_.rotation.y = std::atan2f(direction.x, direction.z);
	float length = std::sqrtf(direction.x * direction.x + direction.z * direction.z);
	transform_.rotation.x = std::atan2f(-direction.y, length);

	deathTimer_ = deathTime_; //* deathTimer_ < 0; isAlive_ = false;
	isAlive_ = true;          //* false -> delete

	Collider::SetCollisionShapeSphere({ .radius = {1.0f * kInitScale + 0.1f} });

	Collider::type_ = ColliderType::Type_PlayerBullet;
	Collider::targetType_ = ColliderType::Type_Test;

	Collider::centerPos_ = transform_.GetWorldPos();

}

void PlayerBullet::Update(const Matrix4x4& viewPro) {

	// 速度加算
	transform_.translation += velocity_;

	if (--deathTimer_ < 0) {

		// playerClass -> remove_if(delete)
		isAlive_ = false;
		 NewMoonGame::RemoveCollider(this);
	}

	Collider::centerPos_ = transform_.translation;

	BaseGameObject::Update(viewPro);
}

void PlayerBullet::Draw() {

	BaseGameObject::Draw();
}

void PlayerBullet::OnCollisionEnter(Collider* collider) {

	if (collider->GetType() == ColliderType::Type_Test) {
	}
}

bool PlayerBullet::IsAlive() {

	return isAlive_;
}