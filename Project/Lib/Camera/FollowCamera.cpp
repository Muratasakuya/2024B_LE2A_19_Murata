#include "FollowCamera.h"

#include "Engine/Base/NewMoonGame.h"
#include "Engine/Managers/ImGuiManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*							FollowCamera classMethods
////////////////////////////////////////////////////////////////////////////////*/

void FollowCamera::Init(const WorldTransform* target) {

	target_ = target;

	transform_.Init();
	transform_.eulerRotate.x = 0.25f;

	offset_ = Vector3(0.0f, 7.0f, -20.0f);

}

void FollowCamera::Update() {

	// オフセット分座標をずらす
	transform_.translation = target_->translation + offset_;

	transform_.Update();

	Matrix4x4 rotateMatrix = Matrix4x4::MakeRotateMatrix(transform_.eulerRotate);
	Matrix4x4 worldMatrix = Matrix4x4::MakeIdentity4x4();
	Matrix4x4 translateMatrix = Matrix4x4::MakeTranslateMatrix(transform_.translation);
	worldMatrix = rotateMatrix * translateMatrix;

	Matrix4x4 viewMatrix = Matrix4x4::Inverse(worldMatrix);
	viewProjectionMatrix_ = viewMatrix * NewMoonGame::GameCamera()->GetCamera3D()->GetProjectionMatrix();

}

void FollowCamera::ImGui() {
#ifdef _DEBUG

	ImGui::Separator();
	ImGui::Text("followCamera");

	ImGui::DragFloat3("offset", &offset_.x, 0.01f);
	ImGui::DragFloat3("rotate", &transform_.eulerRotate.x, 0.01f);

#endif // _DEBUG
}