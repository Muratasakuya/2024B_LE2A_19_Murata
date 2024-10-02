#include "CameraManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*									 Main
////////////////////////////////////////////////////////////////////////////////*/
void CameraManager::Initialize() {

	// 2D
	camera2D_ = std::make_unique<Camera2D>();
	camera2D_->Initialize();

	// 3D
	camera3D_ = std::make_unique<Camera3D>();
	camera3D_->Initialize();

	// ConstBuffer初期化
	cameraBuffer_.Initialize();
}
void CameraManager::Update() {

	camera2D_->Update();
	camera3D_->Update();

	// ConstBuffer転送
	cameraBuffer_.Update(camera3D_->GetWorldPos());
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