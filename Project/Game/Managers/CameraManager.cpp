#include "CameraManager.h"

#include "Engine/Managers/ImGuiManager.h"

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

	debugCamera_ = std::make_unique<DebugCamera>();

}
void CameraManager::Update() {

	camera2D_->Update();

	debugCamera_->Update(camera3D_->GetWorldPos(), camera3D_->GetRotate());
	if (debugCamera_->Enable()) {

		camera3D_->SetCamera(debugCamera_->GetViewProjectionMatrix(),debugCamera_->GetTranslate());
	} else {

		camera3D_->Update();
	}

}

void CameraManager::ImGui() {

	if (ImGui::CollapsingHeader("3D Camera")) {
		camera3D_->ImGui();
	}

	ImGui::Separator();
	if (ImGui::CollapsingHeader("Debug Camera")) {
		debugCamera_->ImGui();
	}

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
DebugCamera* CameraManager::GetDebugCamera() const {
	return debugCamera_.get();
}
