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

	railEditor_ = std::make_unique<RailEditor>();
	railEditor_->Init();

	NewMoonGame::GameCamera()->SetUpRailCamera(railEditor_.get(), Vector3::Zero());

	player_ = std::make_unique<Player>();
	player_->Init();

}

void GameScene::Update() {

	railEditor_->Update();

	player_->Update(NewMoonGame::GameCamera()->GetCamera3D()->GetViewProjectionMatrix());

	if (NewMoonGame::TriggerKey(DIK_SPACE)) {

		SceneManager::GetInstance()->SetNextScene("Title");
	}

}

void GameScene::BackDraw() {

	NewMoonGame::DrawGrid();
	railEditor_->Draw();
	player_->Draw();

}

void GameScene::FrontDraw() {
}

void GameScene::Cleanup() {
}