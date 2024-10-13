#include "CameraManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*									 Main
////////////////////////////////////////////////////////////////////////////////*/
void CameraManager::Init() {

	// 2D
	camera2D_ = std::make_unique<Camera2D>();
	camera2D_->Init();

	// 3D
	camera3D_ = std::make_unique<Camera3D>();
	camera3D_->Init();

	// ConstBuffer初期化
	cameraBuffer_.Init();
	viewProjectionBuffer_.Init();
}
void CameraManager::Update() {

	camera2D_->Update();
	camera3D_->Update();

	// ConstBuffer転送
	cameraBuffer_.Update(camera3D_->GetWorldPos());
	viewProjectionBuffer_.Update(camera3D_->GetViewProjectionMatrix());
}

void CameraManager::ImGui() {

	camera3D_->ImGui();
}

/*////////////////////////////////////////////////////////////////////////////////
*									Getter
////////////////////////////////////////////////////////////////////////////////*/
Matrix4x4 CameraManager::GetViewProjection(CameraType cameraType) {

	switch (cameraType) {
	case CameraType::Perspective:

		// 3D
		return camera3D_->GetViewProjectionMatrix();
		break;
	case CameraType::Orthographic:

		// 2D
		return camera2D_->GetViewProjectionMatrix();
		break;
	}

	return Matrix4x4::MakeIdentity4x4();
}
CameraObject CameraManager::GetCameraBuffer() const { return cameraBuffer_; }
ViewProjectionBuffer CameraManager::GetViewProjectionBuffer() const { return viewProjectionBuffer_; }
