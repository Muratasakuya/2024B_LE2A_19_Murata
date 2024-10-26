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

		Draw();

		NewMoonGame::Renderer2D();

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

	NewMoonGame::GameCamera()->SetUpRailCamera(railEditor_.get(), { 0.0f,0.0f,3.0f });

	player_ = std::make_unique<Player>();
	player_->Init();

}

void GameScene::Update() {

	NewMoonGame::DrawGrid();

	railEditor_->Update();

	player_->Update(NewMoonGame::GameCamera()->GetCamera3D()->GetViewProjectionMatrix());
}

void GameScene::Draw() {

	railEditor_->Draw();

	player_->Draw();

}

void GameScene::Cleanup() {
}