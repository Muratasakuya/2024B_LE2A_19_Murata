#include "GameScene.h"

#include "Engine/Base/NewMoon.h"
#include "Engine/Base/NewMoonGame.h"
#include "Engine/Managers/ImGuiManager.h"
#include "Game/Managers/SceneManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*									Main
////////////////////////////////////////////////////////////////////////////////*/
void GameScene::Init() {

	pointA_ = { -10.0f,0.0f,5.0f };
	pointB_ = { 10.0f,0.0f,5.0f };

	color_ = { 1.0f,1.0f,1.0f,1.0f };
}

void GameScene::Update() {

	ImGui();
}

void GameScene::Draw() {
	//===================================================================*/
	/// Back

	NewMoonGame::DrawLine(pointA_, pointB_, color_);

	NewMoon::ClearDepthBuffer();
	//===================================================================*/
	/// Front



}

void GameScene::ImGui() {
#ifdef _DEBUG
	ImGui::Begin("test");

	ImGui::DragFloat3("pointA", &pointA_.x, 0.01f);
	ImGui::DragFloat3("pointB", &pointB_.x, 0.01f);
	ImGui::ColorEdit4("color", &color_.x);
	ImGui::Text("FrameRate: %.1f FPS", ImGui::GetIO().Framerate);

	ImGui::End();
#endif // _DEBUG
}