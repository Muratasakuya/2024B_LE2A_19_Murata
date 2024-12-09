#include "MoveUtility.h"

#include "Engine/Base/NewMoonGame.h"

/*////////////////////////////////////////////////////////////////////////////////
*								MoveUtility classMethods
////////////////////////////////////////////////////////////////////////////////*/

void MoveUtility::AllKey() {

	keyInfo =
		KeyInformation::MoveLeft |
		KeyInformation::MoveRight |
		KeyInformation::MoveForward |
		KeyInformation::MoveBackward;
}

void MoveUtility::KeyMove(BaseTransform& transform) {

	move_.Init();

	if (static_cast<bool>(keyInfo & KeyInformation::MoveLeft)) {
		if (NewMoonGame::PushKey(DIK_A)) {

			move_.x -= velocity.x;
		}
	}
	if (static_cast<bool>(keyInfo & KeyInformation::MoveRight)) {
		if (NewMoonGame::PushKey(DIK_D)) {

			move_.x += velocity.x;
		}
	}

	if (static_cast<bool>(keyInfo & KeyInformation::MoveForward)) {
		if (NewMoonGame::PushKey(DIK_W)) {

			move_.z += velocity.z;
		}
	}
	if (static_cast<bool>(keyInfo & KeyInformation::MoveBackward)) {
		if (NewMoonGame::PushKey(DIK_S)) {

			move_.z -= velocity.z;
		}
	}

	transform.translation += move_;
}

void MoveUtility::RotateToDirection(BaseTransform& transform, float lerpRate) {

	Vector3 direction = Vector3(move_.x, 0.0f, move_.z).Normalize();

	if (direction.Length() <= 0.0f) {
		return;
	}

	Quaternion targetRotation = Quaternion::LookRotation(direction, Direction::Up());
	transform.rotation = Quaternion::Slerp(transform.rotation, targetRotation, lerpRate);

}

void MoveUtility::Jump(BaseTransform& transform, BYTE keyNumber) {

	// 地面に着いているときのみ
	if (isGrounded_ && NewMoonGame::TriggerKey(keyNumber)) {

		velocity.y = jumpStrength;
		isGrounded_ = false;
	}

	if (!isGrounded_) {

		velocity.y += gravity_ * NewMoonGame::GetDeltaTime();
		transform.translation.y += velocity.y * NewMoonGame::GetDeltaTime();

		if (transform.translation.y <= groundY) {

			transform.translation.y = groundY;
			velocity.y = 0.0f;
			isGrounded_ = true;
		}
	}

}