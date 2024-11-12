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
	NewMoonGame::LoadModel(baseModelDirectory_, "welcome.obj");

	//===================================================================*/
	//* texture *//

	NewMoonGame::LoadTexture("white");
	NewMoonGame::LoadTexture("bulletTargetReticle");
	NewMoonGame::LoadTexture("score");
	NewMoonGame::LoadTexture("scoreNumber");
	NewMoonGame::LoadTexture("scoreBack");

	//===================================================================*/
	//* sound *//

	NewMoonGame::LoadWave("./Resources/Sound/maou_game_vehicle03.wav");
	NewMoonGame::PlayWave("maou_game_vehicle03", true);
	NewMoonGame::SetVolume("maou_game_vehicle03", 0.1f);

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

	welcome_ = std::make_unique<Welcome>();
	welcome_->Init();

	cylinderCollection_ = std::make_unique<CylinderCollection>();
	cylinderCollection_->Init();

	railScore_ = std::make_unique<RailScore>();
	railScore_->Init();

	fireworkCollection_ = std::make_unique<FireworkCollection>();
	fireworkCollection_->Init(railEditor_.get());

	game_ = false;

}

void GameScene::Update() {

	if (NewMoonGame::GameCamera()->GetRailCamera()->IsStart() && !game_) {

		enemyManager_->Reset();
		game_ = true;
	}
	if (!NewMoonGame::GameCamera()->GetRailCamera()->IsStart() && game_) {

		game_ = false;
	}

	railEditor_->Update();

	player_->SetForward(NewMoonGame::GameCamera()->GetRailCamera()->GetFoward());
	player_->Update(NewMoonGame::GameCamera()->GetCamera3D()->GetViewProjectionMatrix());

	enemyManager_->Update();

	ground_->Update();

	welcome_->Update();

	cylinderCollection_->Update();

	railScore_->SetScore(enemyManager_->GetScore());
	railScore_->Update();

	fireworkCollection_->Update();

}

void GameScene::Draw() {

	railEditor_->Draw();

	ground_->Draw();

	welcome_->Draw();

	cylinderCollection_->Draw();

	enemyManager_->Draw();

	player_->Draw();

	railScore_->Draw();

	fireworkCollection_->Draw();

	player_->ReticleDraw();

}

void GameScene::Cleanup() {
}