#include "Camera3D.h"

#include "Engine/Base/NewMoon.h"
#include "Engine/Managers/ImGuiManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*							Camera3D classMethods
////////////////////////////////////////////////////////////////////////////////*/

void Camera3D::Init() {

	// Affine
	data_.transform.scale.SetInit(1.0f);
	data_.transform.rotate = { 0.26f,0.0f,0.0f };
	data_.transform.translate = { 0.0f,4.9f,-15.0f };

	data_.matrix =
		Matrix4x4::MakeAffineMatrix(data_.transform.scale, data_.transform.rotate, data_.transform.translate);
	data_.viewMatrix = Matrix4x4::Inverse(data_.matrix);

	data_.projectionMatrix =
		Matrix4x4::MakePerspectiveFovMatrix(0.45f, NewMoon::kWindowWidthf / NewMoon::kWindowHeightf, 0.1f, 100.0f);

	data_.projectionInverseMatrix = Matrix4x4::Inverse(data_.projectionMatrix);

	// 値の保持
	resetData_ = data_;

	// ConstBuffer初期化
	cameraBuffer_.Init();
	viewProBuffer_.Init();
}

void Camera3D::Update() {

	data_.matrix =
		Matrix4x4::MakeAffineMatrix(data_.transform.scale, data_.transform.rotate, data_.transform.translate);
	data_.viewMatrix = Matrix4x4::Inverse(data_.matrix);

	data_.viewProjectionMatrix = data_.viewMatrix * data_.projectionMatrix;

	// ConstBuffer転送
	cameraBuffer_.Update(data_.transform.translate);
	viewProBuffer_.Update(data_.viewProjectionMatrix);
}

/*////////////////////////////////////////////////////////////////////////////////
*									ImGui
////////////////////////////////////////////////////////////////////////////////*/
void Camera3D::ImGui() {
#ifdef _DEBUG
	ImGui::SliderAngle("Rotate X", &data_.transform.rotate.x);
	ImGui::SliderAngle("Rotate Y", &data_.transform.rotate.y);
	ImGui::SliderAngle("Rotate Z", &data_.transform.rotate.z);
	ImGui::DragFloat3("Translate##Default", &data_.transform.translate.x, 0.01f);
#endif
}

void Camera3D::Reset() {

	data_ = resetData_;
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
void Camera3D::SetViewMatrix(const Matrix4x4& viewMatrix) { data_.viewMatrix = viewMatrix; }
void Camera3D::SetProjectionMatrix(const Matrix4x4& projectionMatrix) { data_.projectionMatrix = projectionMatrix; }
void Camera3D::SetTranslate(const Vector3& translate) { data_.transform.translate = translate; }
void Camera3D::SetRotate(const Vector3& rotate) { data_.transform.rotate = rotate; }

void Camera3D::SetCamera(const Matrix4x4& viewProMatrix, const Vector3& pos) {

	data_.viewProjectionMatrix = viewProMatrix;

	cameraBuffer_.Update(pos);
	viewProBuffer_.Update(data_.viewProjectionMatrix);
}

/*///////////////////////////////////////////////////////////////////////////////
*									Getter
////////////////////////////////////////////////////////////////////////////////*/
Vector3 Camera3D::GetWorldPos() const { return data_.transform.translate; }
Vector3 Camera3D::GetRotate() const { return data_.transform.rotate; }
Matrix4x4 Camera3D::GetCameraMatrix() const { return data_.matrix; }
Matrix4x4 Camera3D::GetViewMatrix() const { return data_.viewMatrix; }
Matrix4x4 Camera3D::GetProjectionMatrix() const { return data_.projectionMatrix; }
Matrix4x4 Camera3D::GetViewProjectionMatrix() const { return data_.viewProjectionMatrix; }
Matrix4x4 Camera3D::GetProjectionInverseMatrix() const { return data_.projectionInverseMatrix; }
CameraObject Camera3D::GetCameraBuffer() const { return cameraBuffer_; }
ViewProjectionBuffer Camera3D::GetViewProBuffer() const { return viewProBuffer_; }