#include "MainCamera3D.h"

#include "Engine/Managers/ImGuiManager.h"
#include "Game/Managers/SceneManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*									Main
////////////////////////////////////////////////////////////////////////////////*/
void MainCamera3D::Initialize() {

	// Affine
	transform_.scale.SetInitialize(1.0f);
	transform_.rotate.Initialize();
	transform_.translate = { 0.0f,0.0f,0.0f };

	cameraMatrix_ =
		Matrix4x4::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	viewMatrix_ = Matrix4x4::Inverse(cameraMatrix_);

	projectionMatrix_ =
		Matrix4x4::MakePerspectiveFovMatrix(0.45f, NewMoon::kWindowWidthf / NewMoon::kWindowHeightf, 0.1f, 100.0f);

	projectionInverseMatrix_ = Matrix4x4::Inverse(projectionMatrix_);
}

void MainCamera3D::Update() {

	cameraMatrix_ =
		Matrix4x4::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	viewMatrix_ = Matrix4x4::Inverse(cameraMatrix_);

	viewProjectionMatrix_ = viewMatrix_ * projectionMatrix_;
}

/*////////////////////////////////////////////////////////////////////////////////
*									ImGui
////////////////////////////////////////////////////////////////////////////////*/
void MainCamera3D::ImGui() {
#ifdef _DEBUG
	ImGui::Begin("3DCamera");

	ImGui::SliderAngle("rotateX", &transform_.rotate.x);
	ImGui::SliderAngle("rotateY", &transform_.rotate.y);
	ImGui::SliderAngle("rotateZ", &transform_.rotate.z);
	ImGui::DragFloat3("translate", &transform_.translate.x, 0.01f);

	ImGui::End();
#endif
}

/*///////////////////////////////////////////////////////////////////////////////
*								getterMethods
////////////////////////////////////////////////////////////////////////////////*/
Vector3 MainCamera3D::GetWorldPos() const { return transform_.translate; }

Matrix4x4 MainCamera3D::GetCameraMatrix() const { return cameraMatrix_; }

Matrix4x4 MainCamera3D::GetViewMatrix() const { return viewMatrix_; }

Matrix4x4 MainCamera3D::GetProjectionMatrix() const { return projectionMatrix_; }

Matrix4x4 MainCamera3D::GetViewProjectionMatrix() const { return viewProjectionMatrix_; }

Matrix4x4 MainCamera3D::GetProjectionInverseMatrix() const { return projectionInverseMatrix_; }