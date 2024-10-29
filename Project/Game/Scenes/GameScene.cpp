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

void GameScene::Load() {

	//* player *//

	NewMoonGame::LoadModel("./Resources/Obj", "cube.obj");
	NewMoonGame::LoadModel("./Resources/Obj", "sphere.obj");
	NewMoonGame::LoadTexture("white");
	NewMoonGame::LoadTexture("bulletTargetReticle");

}

void GameScene::Init() {

	Load();

	railEditor_ = std::make_unique<RailEditor>();
	railEditor_->Init();

	NewMoonGame::GameCamera()->SetUpRailCamera(railEditor_.get(), { 0.0f,0.0f,3.0f });

	player_ = std::make_unique<Player>();
	player_->Init();

}

void GameScene::Update() {

	railEditor_->Update();

	player_->SetForward(NewMoonGame::GameCamera()->GetRailCamera()->GetFoward());
	player_->Update(NewMoonGame::GameCamera()->GetCamera3D()->GetViewProjectionMatrix());

}

void GameScene::Draw() {

	railEditor_->Draw();

	player_->Draw();

}

void GameScene::Cleanup() {
}