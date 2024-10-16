#include "GameScene.h"

#include "Engine/Base/NewMoon.h"
#include "Engine/Base/NewMoonGame.h"
#include "Engine/Managers/ImGuiManager.h"
#include "Game/Managers/SceneManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*							GameScene classMethods
////////////////////////////////////////////////////////////////////////////////*/

GameScene::GameScene() {}
GameScene::~GameScene() {}

void GameScene::Init() {

	railEditor_ = std::make_unique<RailEditor>();
	railEditor_->Init();

	// レールカメラのセット
	NewMoonGame::GameCamera()->SetUpRailCamera(railEditor_.get(), { 0.0f,0.0f,3.0f });

	player_ = std::make_unique<Player>();
	player_->Init();
	// レールカメラとの親子関係
	player_->SetParent(&NewMoonGame::GameCamera()->GetRailCamera()->GetWorldTransform());

}

void GameScene::Update() {

	railEditor_->Update();

	player_->Update(NewMoonGame::GameCamera()->GetCamera3D()->GetViewProjectionMatrix());

}

void GameScene::Draw() {
	//===================================================================*/
	/// Back

	NewMoonGame::DrawGrid();

	railEditor_->Draw();

	player_->Draw();

	NewMoon::ClearDepthBuffer();
	//===================================================================*/
	/// Front



}