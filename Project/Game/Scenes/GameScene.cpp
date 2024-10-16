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

}

void GameScene::Update() {

}

void GameScene::Draw() {
	//===================================================================*/
	/// Back

	NewMoonGame::DrawGrid();

	NewMoon::ClearDepthBuffer();
	//===================================================================*/
	/// Front



}