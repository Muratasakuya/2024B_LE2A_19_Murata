#include "GameScene.h"

#include "Engine/Base/NewMoon.h"
#include "Engine/Base/NewMoonGame.h"
#include "Engine/Managers/ImGuiManager.h"
#include "Game/Managers/SceneManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*							GameScene classMethods
////////////////////////////////////////////////////////////////////////////////*/

GameScene::GameScene() {}
GameScene::~GameScene() {
	int deleteTest = 1;
	deleteTest = 0;
}

void GameScene::Run() {

	Init();

	while (NewMoon::ProcessMessage() == 0) {
		NewMoon::BeginFrame();
		NewMoonGame::Update();

		Update();

		NewMoon::BeginPreOffscreen();
		Draw();
		NewMoon::EndPostOffscreen();

		NewMoon::OffscreenDraw();

		NewMoonGame::Reset();
		NewMoon::EndFrame();

		if (SceneManager::GetInstance()->IsSceneSwitching()) {

			break;
		}

	}

	Cleanup();

}

void GameScene::Load() {
}

void GameScene::Init() {

	Load();

	//================================================================================
	//* SceneMethod *//
	//================================================================================

	sceneName_ = "Game";

}

void GameScene::Update() {

}

void GameScene::Draw() {

	NewMoonGame::DrawGrid();

}

void GameScene::Cleanup() {

	NewMoonGame::ClearAllGameInformation();
}