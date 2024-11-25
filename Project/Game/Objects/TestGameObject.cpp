#include "TestGameObject.h"

#include "Engine/Base/NewMoonGame.h"

/*////////////////////////////////////////////////////////////////////////////////
*							TestGameObject classMethods
////////////////////////////////////////////////////////////////////////////////*/

TestGameObject::~TestGameObject() {
	NewMoonGame::RemoveCollider(this);
}

void TestGameObject::Init() {

	BaseGameObject::Init("cube.obj");
	BaseGameObject::SetName("testGameObject");

	Collider::SetCollisionShapeOBB();
	Collider::size_.SetInit(1.0f);

	Collider::type_ = ColliderType::Type_None;
	Collider::targetType_ = ColliderType::Type_Test;

}

void TestGameObject::Update() {

	Collider::centerPos_ = transform_.translation;
	Collider::rotate_ = transform_.rotation;

	Collider::OBBUpdate();

	BaseGameObject::Update();

}

void TestGameObject::Draw() {

	Collider::DrawCollider();

	BaseGameObject::Draw();
}

void TestGameObject::KeyMove() {
}

void TestGameObject::DerivedImGui() {
#ifdef _DEBUG
#endif // _DEBUG
}

void TestGameObject::OnCollisionEnter(Collider* other) {

	if (other->GetType() == Collider::GetTargetType()) {

		int test = 1;
		test = 0;
	}
}
void TestGameObject::OnCollisionStay(Collider* other) {

	if (other->GetType() == Collider::GetTargetType()) {

		int test = 1;
		test = 0;
	}
}
void TestGameObject::OnCollisionExit(Collider* other) {

	if (other->GetType() == Collider::GetTargetType()) {

		int test = 1;
		test = 0;
	}
}