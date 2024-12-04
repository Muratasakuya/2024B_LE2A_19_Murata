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

	NewMoonGame::LoadTexture("checkerBoard");

	NewMoonGame::LoadModel(baseModelDirectory_, "cube.obj");

}

void TitleScene::Init() {

	Load();

	objects_ = std::make_unique<TestGameObject>();
	objects_->Init();

	field_ = std::make_unique<TemplateField>();
	field_->Init();

	particle_ = std::make_unique<TestParticle>();
	particle_->Init();

	sceneName_ = "Title";

}

void TitleScene::Update() {

	objects_->Update();

	field_->Update();

	particle_->Update();

}

void TitleScene::Draw() {

	objects_->Draw();

	field_->Draw();

	particle_->Draw(kBlendModeAdd);

}

void TitleScene::Cleanup() {

	NewMoonGame::ClearAllGameInformation();
}