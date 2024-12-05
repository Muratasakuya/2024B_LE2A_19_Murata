#include "TestGameObject.h"

#include "Engine/Base/NewMoonGame.h"
#include "Engine/Managers/ImGuiManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*							TestGameObject classMethods
////////////////////////////////////////////////////////////////////////////////*/

TestGameObject::~TestGameObject() {

	NewMoonGame::RemoveCollider(this);
	NewMoonGame::EraseToImGui(this);
}

void TestGameObject::Init(uint32_t index) {

	BaseGameObject::Init("cube.obj");

	BaseGameObject::SetName("testGameObject", index);

	// json適応
	/*BaseGameObject::ApplyJsonForColor();
	BaseGameObject::ApplyJsonForTransform(transform_);*/

	Collider::SetCollisionShapeOBB();

	// log用
	Collider::name_ = BaseGameObject::GetName();

}

void TestGameObject::Update() {

	Collider::centerPos_ = transform_.translation;
	Collider::size_ = Vector3(transform_.scale.x + 0.1f, transform_.scale.y + 0.1f, transform_.scale.z + 0.1f);
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

	ImGui::DragFloat3("rotate", &transform_.eulerRotate.x, 0.01f);
	transform_.rotation = Quaternion::EulerToQuaternion(transform_.eulerRotate);

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