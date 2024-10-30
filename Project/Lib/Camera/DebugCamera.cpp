#include "DebugCamera.h"

#include "Engine/Base/NewMoon.h"
#include "Engine/Base/NewMoonGame.h"
#include "Engine/Managers/ImGuiManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*						DebugCamera classMethods
////////////////////////////////////////////////////////////////////////////////*/

DebugCamera::DebugCamera() {

	// 最初は無効
	enable_ = false;

	//* default *//

	zoomRate_ = 1.0f;
}

void DebugCamera::Update(const Vector3& pos, const Vector3& rotate) {

	if (!enable_) {

		// 無効な時は今のカメラの位置を取得する
		translation_ = pos;
		rotation_ = rotate;
		return;
	}

	Move();

	matrix_ = Matrix4x4::MakeIdentity4x4();
	Vector3 offset = { 0.0f, 2.0f, -25.0f };
	Vector3 translate{};
	offset = Vector3::TransferNormal(offset, rotateMatrix_);

	matrix_ = Matrix4x4::MakeIdentity4x4();

	Matrix4x4 translateMatrix = Matrix4x4::MakeTranslateMatrix(translation_);
	matrix_ = Matrix4x4::Multiply(rotateMatrix_, translateMatrix);
	Matrix4x4  viewMatrix = Matrix4x4::Inverse(matrix_);

}

void DebugCamera::ImGui() {
#ifdef _DEBUG
	ImGui::Text("DebugCamera");
	ImGui::Checkbox("Enable", &enable_);
	ImGui::DragFloat3("Translate##Debug", &translation_.x, 0.01f);
	ImGui::DragFloat3("Rotation##Debug", &rotation_.x, 0.01f);
	ImGui::DragFloat("ZoomRate", &zoomRate_, 0.01f);
#endif // DEBUG
}

void DebugCamera::Move() {

	float deltaX = NewMoonGame::GetMouseMoveValue().x;
	float deltaY = NewMoonGame::GetMouseMoveValue().y;

	const float rotateSpeed = 0.01f;
	const float panSpeed = 0.05f;

	// 左クリック
	if (NewMoonGame::PushMouseLeft()) {

		rotation_.x += deltaY * rotateSpeed;
		rotation_.y += deltaX * rotateSpeed;
	}

	// 中クリック + Shift
	if (NewMoonGame::PushMouseCenter() && NewMoonGame::PushKey(DIK_LSHIFT)) {

		Vector3 right = { panSpeed * deltaX, 0.0f, 0.0f };
		Vector3 up = { 0.0f, -panSpeed * deltaY, 0.0f };

		// 平行移動ベクトルを変換
		right = Vector3::TransferNormal(right, matrix_);
		up = Vector3::TransferNormal(up, matrix_);

		translation_ += right + up;
	}

	Vector3 forward = { 0.0f, 0.0f, NewMoonGame::GetMouseWheel() * zoomRate_ };
	forward = Vector3::TransferNormal(forward, rotateMatrix_);

	if (NewMoonGame::GetMouseWheel() != 0) {

		translation_ += forward; 
	}

	rotateMatrix_ = Matrix4x4::MakeRotateMatrix(rotation_);

}

void DebugCamera::SetEnable(bool enable) {
	enable_ = enable;
}

Vector3 DebugCamera::GetTranslate() const {
	return translation_;
}

Vector3 DebugCamera::GetRotate() const {
	return rotation_;
}

bool DebugCamera::Enable() const {
	return enable_;
}