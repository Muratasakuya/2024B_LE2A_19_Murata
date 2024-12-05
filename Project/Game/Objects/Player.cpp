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

	moveSpeed_ = 0.1f;

	//* particle *//

	chaseParticle_ = std::make_unique<PlayerChaseParticle>();
	chaseParticle_->Init();

}

void Player::Update() {

	// 移動処理
	KeyMove();

	BaseGameObject::Update();

	float offsetDistance = 0.25f;
	Vector3 offset = GetForward() * (-offsetDistance);
	Vector3 chasePos = transform_.GetWorldPos() + offset;

	chaseParticle_->Update();
	chaseParticle_->SetPos(chasePos);

}

void Player::Draw() {

	BaseGameObject::Draw();

	chaseParticle_->Draw(kBlendModeAdd);

}

void Player::KeyMove() {

	float moveX = 0.0f;
	float moveZ = 0.0f;

	if (NewMoonGame::PushKey(DIK_D)) {

		moveX += moveSpeed_;
	} else if (NewMoonGame::PushKey(DIK_A)) {

		moveX -= moveSpeed_;
	}

	if (NewMoonGame::PushKey(DIK_W)) {

		moveZ += moveSpeed_;
	} else if (NewMoonGame::PushKey(DIK_S)) {

		moveZ -= moveSpeed_;
	}

	if (moveX != 0.0f || moveZ != 0.0f) {

		// 移動の適用
		transform_.translation.x += moveX;
		transform_.translation.z += moveZ;

		float length = sqrt(moveX * moveX + moveZ * moveZ);
		if (length != 0.0f) {
			moveX /= length;
			moveZ /= length;
		}

		float angle = atan2(moveX, moveZ);
		const float lerpRate = 0.2f;

		// 動いている方向に回転を合わせる
		transform_.eulerRotate.y = Math::LerpShortAngle(transform_.eulerRotate.y, angle, lerpRate);
		transform_.rotation = Quaternion::EulerToQuaternion(transform_.eulerRotate);
	}
}

void Player::DerivedImGui() {
#ifdef _DEBUG

	ImGui::DragFloat("moveSpeed", &moveSpeed_, 0.01f);

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
