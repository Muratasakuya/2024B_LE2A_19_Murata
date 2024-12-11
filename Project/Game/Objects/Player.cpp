#include "Player.h"

#include "Engine/Base/NewMoonGame.h"
#include "Engine/Managers/ImGuiManager.h"
#include "Lib/Math/Math.h"

/*////////////////////////////////////////////////////////////////////////////////
*	Player ClassMethods
////////////////////////////////////////////////////////////////////////////////*/

void Player::Init() {

	BaseGameObject::Init("cube.obj");

	BaseGameObject::SetMeshRenderer("player");

	parentFolderName_ = "./Player/";

	BaseGameObject::ApplyJsonForColor();
	BaseGameObject::ApplyJsonForTransform(transform_);

	move_.AllKey();
	move_.velocity.x = 0.1f;
	move_.velocity.z = 0.1f;
	move_.jumpStrength = 5.0f;
	move_.groundY = transform_.scale.y;

	Collider::SetCollisionShapeOBB();

}

void Player::Update() {

	// 移動処理
	Move();

	Collider::OBBUpdate();

	Collider::rotate_ = transform_.rotation;
	Collider::centerPos_ = transform_.translation;
	Collider::size_ = Vector3(transform_.scale.x + 0.1f, transform_.scale.y + 0.1f, transform_.scale.z + 0.1f);

	BaseGameObject::Update();

}

void Player::Draw() {

	Collider::DrawCollider();

}

void Player::Move() {

	move_.GamePadMove(transform_);
	move_.GamePadJamp(transform_, InputGamePadButtons::A);
	move_.RotateToDirection(transform_);

}

void Player::DerivedImGui() {
#ifdef _DEBUG

	ImGui::Text("jumpParameter");
	ImGui::DragFloat("strength", &move_.jumpStrength, 0.01f);

#endif // _DEBUG
}

Vector3 Player::GetForward() const {

	Vector3 forward = Direction::Forward();

	Quaternion rotation = transform_.rotation;
	forward = rotation * forward;

	return forward;
}
