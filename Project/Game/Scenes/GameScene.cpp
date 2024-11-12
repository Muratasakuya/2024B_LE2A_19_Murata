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

	//===================================================================*/
	//* primitive *//

	NewMoonGame::LoadModel(baseModelDirectory_, "cube.obj");
	NewMoonGame::LoadModel(baseModelDirectory_, "sphere.obj");

	//===================================================================*/
	//* enemy *//

	NewMoonGame::LoadModel(baseModelDirectory_, "balloon.obj"); // 風船
	NewMoonGame::LoadModel(baseModelDirectory_, "UFO.obj");     // UFO
	NewMoonGame::LoadModel(baseModelDirectory_, "star.obj");    // 星
	NewMoonGame::LoadModel(baseModelDirectory_, "ghost.obj");   // お化け

	//===================================================================*/
	//* field *//

	NewMoonGame::LoadModel(baseModelDirectory_, "ground.obj");
	NewMoonGame::LoadModel(baseModelDirectory_, "cylinder.obj");

	//===================================================================*/
	//* texture *//

	NewMoonGame::LoadTexture("white");
	NewMoonGame::LoadTexture("bulletTargetReticle");
	NewMoonGame::LoadTexture("score");
	NewMoonGame::LoadTexture("scoreNumber");
	NewMoonGame::LoadTexture("scoreBack");

}

void GameScene::Init() {

	Load();

	railEditor_ = std::make_unique<RailEditor>();
	railEditor_->Init();

	NewMoonGame::GameCamera()->SetUpRailCamera(railEditor_.get(), { 0.0f,0.0f,3.0f });

	player_ = std::make_unique<Player>();
	player_->Init();

	enemyManager_ = std::make_unique<EnemyManager>();
	enemyManager_->Init(railEditor_.get());

	ground_ = std::make_unique<Ground>();
	ground_->Init();

	cylinderCollection_ = std::make_unique<CylinderCollection>();
	cylinderCollection_->Init();

	railScore_ = std::make_unique<RailScore>();
	railScore_->Init();

}

void GameScene::Update() {

	railEditor_->Update();

	player_->SetForward(NewMoonGame::GameCamera()->GetRailCamera()->GetFoward());
	player_->Update(NewMoonGame::GameCamera()->GetCamera3D()->GetViewProjectionMatrix());

	enemyManager_->Update();

	ground_->Update();

	cylinderCollection_->Update();

	railScore_->SetScore(enemyManager_->GetScore());
	railScore_->Update();

}

void GameScene::Draw() {

	railEditor_->Draw();

	ground_->Draw();

	cylinderCollection_->Draw();

	enemyManager_->Draw();

	player_->Draw();

	railScore_->Draw();

}

void GameScene::Cleanup() {
}