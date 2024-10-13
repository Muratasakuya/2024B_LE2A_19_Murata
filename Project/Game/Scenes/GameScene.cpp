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

	if (NewMoonGame::TriggerKey(DIK_SPACE)) {

		SceneManager::GetInstance()->ChangeScene(TITLE, TransitionType::Fade);
	}

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
	
#endif // _DEBUG
}