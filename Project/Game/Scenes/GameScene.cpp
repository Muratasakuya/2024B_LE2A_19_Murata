#include "GameScene.h"

#include "Engine/Base/NewMoon.h"
#include "Engine/Base/NewMoonGame.h"
#include "Engine/Managers/ImGuiManager.h"
#include "Game/Managers/SceneManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*									Main
////////////////////////////////////////////////////////////////////////////////*/
void GameScene::Init() {

}

void GameScene::Update() {

	ImGui();
}

void GameScene::Draw() {
	//===================================================================*/
	/// Back

	NewMoonGame::DrawGrid();

	NewMoon::ClearDepthBuffer();
	//===================================================================*/
	/// Front



}

void GameScene::ImGui() {
#ifdef _DEBUG
	ImGui::Begin("test");

	ImGui::Text("FrameRate: %.1f FPS", ImGui::GetIO().Framerate);

	ImGui::End();
#endif // _DEBUG
}