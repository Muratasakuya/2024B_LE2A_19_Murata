#include "Camera3D.h"

#include "Engine/Base/NewMoon.h"
#include "Engine/Managers/ImGuiManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*							Camera3D classMethods
////////////////////////////////////////////////////////////////////////////////*/

void Camera3D::Init() {

	// Affine
	data_.transform.scale.SetInit(1.0f);
	data_.transform.rotation = { 0.0f,0.0f,0.0f };
	data_.transform.translation = { 0.0f,1.65f,-21.0f };

	data_.matrix =
		Matrix4x4::MakeAffineMatrix(data_.transform.scale, data_.transform.rotation, data_.transform.translation);
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
		Matrix4x4::MakeAffineMatrix(data_.transform.scale, data_.transform.rotation, data_.transform.translation);
	data_.viewMatrix = Matrix4x4::Inverse(data_.matrix);

	data_.viewProjectionMatrix = data_.viewMatrix * data_.projectionMatrix;

	// ConstBuffer転送
	cameraBuffer_.Update(data_.transform.translation);
	viewProBuffer_.Update(data_.viewProjectionMatrix);
}

/*////////////////////////////////////////////////////////////////////////////////
*									ImGui
////////////////////////////////////////////////////////////////////////////////*/
void Camera3D::ImGui() {
#ifdef _DEBUG
	ImGui::Text("MainCamera");
	ImGui::SliderAngle("Rotate X", &data_.transform.rotation.x);
	ImGui::SliderAngle("Rotate Y", &data_.transform.rotation.y);
	ImGui::SliderAngle("Rotate Z", &data_.transform.rotation.z);
	ImGui::DragFloat3("Translate##Default", &data_.transform.translation.x, 0.01f);
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
void Camera3D::SetViewProjectionMatrix(const Matrix4x4& viewProMatrix) { data_.viewProjectionMatrix = viewProMatrix; }
void Camera3D::SetTranslate(const Vector3& translate) { data_.transform.translation = translate; }
void Camera3D::SetRotate(const Vector3& rotate) { data_.transform.rotation = rotate; }

/*///////////////////////////////////////////////////////////////////////////////
*									Getter
////////////////////////////////////////////////////////////////////////////////*/
Vector3 Camera3D::GetWorldPos() const { return data_.transform.translation; }
Vector3 Camera3D::GetRotate() const { return data_.transform.rotation; }
Matrix4x4 Camera3D::GetCameraMatrix() const { return data_.matrix; }
Matrix4x4 Camera3D::GetViewMatrix() const { return data_.viewMatrix; }
Matrix4x4 Camera3D::GetProjectionMatrix() const { return data_.projectionMatrix; }
Matrix4x4 Camera3D::GetViewProjectionMatrix() const { return data_.viewProjectionMatrix; }
Matrix4x4 Camera3D::GetProjectionInverseMatrix() const { return data_.projectionInverseMatrix; }
CameraObject Camera3D::GetCameraBuffer() const { return cameraBuffer_; }
ViewProjectionBuffer Camera3D::GetViewProBuffer() const { return viewProBuffer_; }