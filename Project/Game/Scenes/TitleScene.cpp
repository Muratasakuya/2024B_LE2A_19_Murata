#include "TitleScene.h"

#include "Engine/Base/NewMoon.h"
#include "Engine/Base/NewMoonGame.h"
#include "Engine/Managers/ImGuiManager.h"
#include "Game/Managers/SceneManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*							TitleScene classMethods
////////////////////////////////////////////////////////////////////////////////*/

TitleScene::TitleScene() {}
TitleScene::~TitleScene() {
	int deleteTest = 1;
	deleteTest = 0;
}

void TitleScene::Run() {

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


void TitleScene::Load() {

	NewMoonGame::LoadTexture("templateField");

	NewMoonGame::LoadModel(baseModelDirectory_, "cube.obj");

}

void TitleScene::Init() {

	Load();

	field_ = std::make_unique<TemplateField>();
	field_->Init();
	
	player_ = std::make_unique<Player>();
	player_->Init();

	NewMoonGame::GameCamera()->GetFollowCamera()->Init(&player_->GetWorldTransform());

	//================================================================================
	//* SceneMethod *//
	//================================================================================

	sceneName_ = "Title";

}

void TitleScene::Update() {

	field_->Update();

	player_->Update();

}

void TitleScene::Draw() {

	field_->Draw();

	player_->Draw();

}

void TitleScene::Cleanup() {

	NewMoonGame::ClearAllGameInformation();
}