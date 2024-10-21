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
	int test = 1;
	test = 0;
}

void GameScene::Run() {

	Init();

	while (NewMoon::ProcessMessage() == 0) {
		NewMoon::BeginFrame();
		NewMoonGame::Update();

		Update();

		BackDraw();
		NewMoon::ClearDepthBuffer();
		FrontDraw();

		NewMoonGame::Reset();
		NewMoon::EndFrame();

		if (SceneManager::GetInstance()->IsSceneSwitching()) {
			break;
		}
	}

	Cleanup();

}

void GameScene::Init() {

}

void GameScene::Update() {

}

void GameScene::BackDraw() {

	NewMoonGame::DrawGrid();

}

void GameScene::FrontDraw() {
}

void GameScene::Cleanup() {
}