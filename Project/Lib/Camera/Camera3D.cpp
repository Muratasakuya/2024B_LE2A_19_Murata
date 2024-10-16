#include "Camera3D.h"

#include "Engine/Managers/ImGuiManager.h"
#include "Game/Managers/SceneManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*							Camera3D classMethods
////////////////////////////////////////////////////////////////////////////////*/

void Camera3D::Init() {

	// Affine
	transform_.scale.SetInit(1.0f);
	transform_.rotate = { 0.26f,0.0f,0.0f };
	transform_.translate = { 0.0f,4.9f,-15.0f };

	cameraMatrix_ =
		Matrix4x4::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	viewMatrix_ = Matrix4x4::Inverse(cameraMatrix_);

	projectionMatrix_ =
		Matrix4x4::MakePerspectiveFovMatrix(0.45f, NewMoon::kWindowWidthf / NewMoon::kWindowHeightf, 0.1f, 100.0f);

	projectionInverseMatrix_ = Matrix4x4::Inverse(projectionMatrix_);

	// ConstBuffer初期化
	cameraBuffer_.Init();
	viewProBuffer_.Init();
}

void Camera3D::Update() {

	cameraMatrix_ =
		Matrix4x4::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	viewMatrix_ = Matrix4x4::Inverse(cameraMatrix_);

	viewProjectionMatrix_ = viewMatrix_ * projectionMatrix_;

	// ConstBuffer転送
	cameraBuffer_.Update(transform_.translate);
	viewProBuffer_.Update(viewProjectionMatrix_);
}

/*////////////////////////////////////////////////////////////////////////////////
*									ImGui
////////////////////////////////////////////////////////////////////////////////*/
void Camera3D::ImGui() {
#ifdef _DEBUG
	ImGui::SliderAngle("Rotate X", &transform_.rotate.x);
	ImGui::SliderAngle("Rotate Y", &transform_.rotate.y);
	ImGui::SliderAngle("Rotate Z", &transform_.rotate.z);
	ImGui::DragFloat3("Translate", &transform_.translate.x, 0.01f);
#endif
}

void Camera3D::CameraSetCommand(ID3D12GraphicsCommandList* commandList, const PipelineType& pipelineType) {

	cameraBuffer_.SetCommand(commandList, cameraBuffer_.GetRootParameterIndex(pipelineType));
}

void Camera3D::ViewProSetCommand(ID3D12GraphicsCommandList* commandList) {

	viewProBuffer_.SetCommand(commandList, viewProBuffer_.GetRootParameterIndex());
}

/*///////////////////////////////////////////////////////////////////////////////
*									Setter
////////////////////////////////////////////////////////////////////////////////*/
void Camera3D::SetViewMatrix(const Matrix4x4& viewMatrix) { viewMatrix_ = viewMatrix; }

/*///////////////////////////////////////////////////////////////////////////////
*									Getter
////////////////////////////////////////////////////////////////////////////////*/
Vector3 Camera3D::GetWorldPos() const { return transform_.translate; }
Matrix4x4 Camera3D::GetCameraMatrix() const { return cameraMatrix_; }
Matrix4x4 Camera3D::GetViewMatrix() const { return viewMatrix_; }
Matrix4x4 Camera3D::GetProjectionMatrix() const { return projectionMatrix_; }
Matrix4x4 Camera3D::GetViewProjectionMatrix() const { return viewProjectionMatrix_; }
Matrix4x4 Camera3D::GetProjectionInverseMatrix() const { return projectionInverseMatrix_; }
CameraObject Camera3D::GetCameraBuffer() const { return cameraBuffer_; }
ViewProjectionBuffer Camera3D::GetViewProBuffer() const { return viewProBuffer_; }