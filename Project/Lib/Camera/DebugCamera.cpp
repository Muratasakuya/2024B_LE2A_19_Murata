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

	rotateMatrix_ = Matrix4x4::MakeRotateMatrix(rotation_);
	matrix_ = Matrix4x4::MakeIdentity4x4();
	Vector3 offset = { 0.0f, 2.0f, -25.0f };
	Vector3 translate{};
	offset = Vector3::TransferNormal(offset, rotateMatrix_);

	matrix_ = Matrix4x4::MakeIdentity4x4();

	Matrix4x4 translateMatrix = Matrix4x4::MakeTranslateMatrix(translation_);
	Matrix4x4 scaleMatrix = Matrix4x4::MakeScaleMatrix({ 1.0f,1.0f,1.0f });
	matrix_ = Matrix4x4::Multiply(scaleMatrix, rotateMatrix_);
	matrix_ = Matrix4x4::Multiply(matrix_, translateMatrix);
	Matrix4x4  viewMatrix = Matrix4x4::Inverse(matrix_);

	viewProjectionMatrix_ = viewMatrix * NewMoonGame::GameCamera()->GetCamera3D()->GetProjectionMatrix();

}

void DebugCamera::ImGui() {
#ifdef _DEBUG
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
	const float panSpeed = 0.1f;

	// 右クリック
	if (NewMoonGame::PushMouseRight()) {

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
	//Vector3 movement{ 0.0f, 0.0f, 0.0f };
	//// Wキーで前進
	//if (NewMoonGame::PushKey(DIK_W)) {
	//	movement.y += panSpeed;
	//}
	//// Sキーで後退
	//if (NewMoonGame::PushKey(DIK_S)) {
	//	movement.y -= panSpeed;
	//}
	//// Dキーで右移動
	//if (NewMoonGame::PushKey(DIK_D)) {
	//	movement.x += panSpeed;
	//}
	//// Aキーで左移動
	//if (NewMoonGame::PushKey(DIK_A)) {
	//	movement.x -= panSpeed;
	//}
	//// 平行移動ベクトルを変換
	//movement = Vector3::TransferNormal(movement, matrix_);
	//translation_ += movement;

	Vector3 forward = { 0.0f, 0.0f, NewMoonGame::GetMouseWheel() * zoomRate_ };
	forward = Vector3::TransferNormal(forward, rotateMatrix_);

	if (NewMoonGame::GetMouseWheel() != 0) {

		translation_ += forward;
	}

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

Matrix4x4 DebugCamera::GetViewProjectionMatrix() const {
	return viewProjectionMatrix_;
}
