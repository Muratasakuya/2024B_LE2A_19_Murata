#include "Player.h"

#include "Engine/Base/NewMoonGame.h"
#include "Engine/Managers/ImGuiManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*								Player classMethods
////////////////////////////////////////////////////////////////////////////////*/

void Player::Init() {

	const std::string modelName = "cube.obj";
	NewMoonGame::LoadModel("./Resources/Obj/", modelName);
	const std::string textureName = "white";
	NewMoonGame::LoadTexture(textureName);

	BaseModel::Init(modelName);
	BaseModel::model_->SetTexture(textureName);

	BaseModel::transform_.scale = { 0.25f,0.25f,0.25f };

	BaseModel::materials_.front().color = { 1.0f,0.0f,0.0f,1.0f };

}

void Player::Update(const Matrix4x4& viewPro) {

	ImGui();

	BaseModel::transform_.Update(viewPro);
	BaseModel::materials_.front().Update();
}

void Player::Draw() {

	BaseModel::Draw();
}

void Player::ImGui() {
#ifdef _DEBUG
	ImGui::Begin("Player");
	ImGui::DragFloat3("Translate", &BaseModel::transform_.translation.x, 0.01f);
	ImGui::DragFloat3("Rotate", &BaseModel::transform_.rotation.x, 0.01f);
	ImGui::DragFloat3("Scale", &BaseModel::transform_.scale.x, 0.01f);
	ImGui::End();
#endif // _DEBUG
}

/*////////////////////////////////////////////////////////////////////////////////
*										Setter
////////////////////////////////////////////////////////////////////////////////*/
void Player::SetForward(const Vector3& forward) {
	forward_ = forward;
}
void Player::SetParent(const WorldTransform* parent) {
	BaseModel::transform_.parent_ = parent;
}
