#include "Camera2D.h"

#include "Engine/Managers/ImGuiManager.h"
#include "Game/Managers/SceneManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*									Main
////////////////////////////////////////////////////////////////////////////////*/
void Camera2D::Init() {

	// アフィン
	transform_.scale = { 1.0f,1.0f,1.0f };
	transform_.rotate = { 0.0f,0.0f,0.0f };
	transform_.translate = { 0.0f,0.0f,0.0f };

	cameraMatrix_ =
		Matrix4x4::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	viewMatrix_ = Matrix4x4::Inverse(cameraMatrix_);

	orthoMatrix_ =
		Matrix4x4::MakeOrthographicMatrix(0.0f, 0.0f, NewMoon::kWindowWidthf, NewMoon::kWindowHeightf, 0.0f, 100.0f);
}

void Camera2D::Update() {

	cameraMatrix_ =
		Matrix4x4::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	viewMatrix_ = Matrix4x4::Inverse(cameraMatrix_);

	viewProjectionMatrix_ = viewMatrix_ * orthoMatrix_;
}

/*////////////////////////////////////////////////////////////////////////////////
*									ImGui
////////////////////////////////////////////////////////////////////////////////*/
void Camera2D::ImGui() {

	ImGui::Begin("Camera2D");

	ImGui::SliderAngle("rotateX", &transform_.rotate.x);
	ImGui::SliderAngle("rotateY", &transform_.rotate.y);
	ImGui::SliderAngle("rotateZ", &transform_.rotate.z);
	ImGui::SliderFloat3("translate", &transform_.translate.x, -20.0f, 50.0f);

	ImGui::End();
}

/*///////////////////////////////////////////////////////////////////////////////
*									Getter
////////////////////////////////////////////////////////////////////////////////*/
Matrix4x4 Camera2D::GetViewMatrix() const { return viewMatrix_; }
Matrix4x4 Camera2D::GetOrthoMatrix() const { return orthoMatrix_; }
Matrix4x4 Camera2D::GetViewProjectionMatrix() const { return viewProjectionMatrix_; }