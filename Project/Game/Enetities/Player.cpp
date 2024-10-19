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

	BaseGameObject::Init(modelName);
	model_->SetTexture(textureName);

	transform_.scale = { 0.25f,0.25f,0.25f };
	transform_.translation.z = 24.0f;

	transform_.parent_ = &NewMoonGame::GameCamera()->GetRailCamera()->GetTransform();

	BaseGameObject::SetName("Player");

}

void Player::Update(const Matrix4x4& viewPro) {

	BaseGameObject::Update(viewPro);
}

void Player::Draw() {

	BaseGameObject::Draw();
}

/*////////////////////////////////////////////////////////////////////////////////
*										Setter
////////////////////////////////////////////////////////////////////////////////*/
void Player::SetForward(const Vector3& forward) {
	forward_ = forward;
}