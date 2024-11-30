#include "TestGameObject.h"

#include "Engine/Base/NewMoonGame.h"
#include "Engine/Managers/ImGuiManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*							TestGameObject classMethods
////////////////////////////////////////////////////////////////////////////////*/

TestGameObject::~TestGameObject() {
	NewMoonGame::RemoveCollider(this);
}

void TestGameObject::Init() {

	BaseGameObject::Init("cube.obj");

	// TestGameObject or decltype(*this)
	BaseGameObject::AssignIndex<TestGameObject>();
	// 複数自身を生成する場合はAssignIndex()を先に呼びだす
	BaseGameObject::SetName("testGameObject");

	// json適応
	/*BaseGameObject::ApplyJsonForColor();
	BaseGameObject::ApplyJsonForTransform(transform_);*/

	Collider::SetCollisionShapeOBB();

	// log用
	Collider::name_ = BaseGameObject::GetName();

}

void TestGameObject::Update() {

	Collider::centerPos_ = transform_.translation;
	Collider::size_ = transform_.scale;
	Collider::rotate_ = transform_.rotation;

	Collider::OBBUpdate();

	BaseGameObject::Update();

}

void TestGameObject::Draw() {

	Collider::DrawCollider(LineColor::Red);

	BaseGameObject::Draw();
}

void TestGameObject::KeyMove() {
}

void TestGameObject::DerivedImGui() {
#ifdef _DEBUG

	ImGui::DragFloat3("rotate", &transform_.eulerRotate_.x, 0.01f);
	transform_.rotation = Quaternion::EulerToQuaternion(transform_.eulerRotate_);

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