#include "Player.h"

#include "Engine/Base/NewMoonGame.h"
#include "Engine/Managers/ImGuiManager.h"
#include "Lib/Math/Math.h"

/*////////////////////////////////////////////////////////////////////////////////
*								Player classMethods
////////////////////////////////////////////////////////////////////////////////*/

Player::~Player() {
	NewMoonGame::EraseToImGui(this);
}

void Player::Init() {

	BaseGameObject::Init("cube.obj");

	BaseGameObject::SetName("player");

	parentFolderName_ = "./Player/";

	BaseGameObject::ApplyJsonForColor();
	BaseGameObject::ApplyJsonForTransform(transform_);

	move_.AllKey();
	move_.velocity.x = 0.1f;
	move_.velocity.z = 0.1f;
	move_.jumpStrength = 5.0f;
	move_.groundY = transform_.scale.y;

}

void Player::Update() {

	// 移動処理
	KeyMove();

	BaseGameObject::Update();

}

void Player::Draw() {

	BaseGameObject::Draw();

}

void Player::KeyMove() {

	move_.KeyMove(transform_);
	move_.RotateToDirection(transform_);
	move_.Jump(transform_, DIK_SPACE);

}

void Player::DerivedImGui() {
#ifdef _DEBUG

	ImGui::Text("jumpParameter");
	ImGui::DragFloat("strength", &move_.jumpStrength, 0.01f);

#endif // _DEBUG
}

Vector3 Player::GetForward() const {

	float angleY = transform_.eulerRotate.y;
	Vector3 forward;
	forward.x = std::sinf(angleY);
	forward.z = std::cosf(angleY);
	forward.y = 0.0f;

	return forward;
}
