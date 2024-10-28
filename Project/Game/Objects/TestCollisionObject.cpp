#include "TestCollisionObject.h"

/*////////////////////////////////////////////////////////////////////////////////
*						TestCollisionObject classMethods
////////////////////////////////////////////////////////////////////////////////*/

void TestCollisionObject::Init() {

	BaseGameObject::Init("sphere.obj");
	BaseGameObject::SetName("TestCollisionObject");

	//* スケール *//
	const float kInitScale = 1.5f;
	transform_.scale.SetInit(kInitScale);

	Collider::SetCollisionShapeSphere({ .radius = {1.0f * kInitScale + 0.1f} });

	Collider::type_ = ColliderType::Type_Test;
	Collider::targetType_ = ColliderType::Type_PlayerBullet;

	Collider::centerPos_ = transform_.translation;

}

void TestCollisionObject::Update(const Matrix4x4& viewPro) {

	Collider::centerPos_ = transform_.translation;

	BaseGameObject::Update(viewPro);
}

void TestCollisionObject::Draw() {

	BaseGameObject::Draw();
}

void TestCollisionObject::OnCollisionEnter(Collider* collider) {

	if (collider->GetType() == ColliderType::Type_PlayerBullet) {

		int test = 0;
		test = 1;
	}
}

void TestCollisionObject::OnCollisionStay(Collider* collider) {

	if (collider->GetType() == ColliderType::Type_PlayerBullet) {

		int test = 0;
		test = 1;
	}
}

void TestCollisionObject::OnCollisionExit(Collider* collider) {

	if (collider->GetType() == ColliderType::Type_PlayerBullet) {

		int test = 0;
		test = 1;
	}
}