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
}
void CameraManager::Update() {

	camera2D_->Update();
	camera3D_->Update();

	if (railCamera_) {

		railCamera_->Update();
	}
}

void CameraManager::ImGui() {

	camera3D_->ImGui();
	if (railCamera_) {
		railCamera_->ImGui();
	}
}

void CameraManager::SetUpRailCamera(RailEditor* railEditor, const Vector3& initPos) {

	if (railCamera_) {
		return;
	}

	railCamera_ = std::make_unique<RailCamera>();
	railCamera_->Init(railEditor, initPos);
}

/*////////////////////////////////////////////////////////////////////////////////
*									Getter
////////////////////////////////////////////////////////////////////////////////*/
Camera2D* CameraManager::GetCamera2D() const {
	return camera2D_.get();
}
Camera3D* CameraManager::GetCamera3D() const {
	return camera3D_.get();
}
RailCamera* CameraManager::GetRailCamera() const{

	return railCamera_.get();
}
