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

	NewMoonGame::LoadTexture("screenSizeWhite");

	NewMoonGame::LoadModel(baseModelDirectory_, "cube.obj");

}

void TitleScene::Init() {

	Load();

	for (uint32_t index = 0; index < 2; ++index) {

		auto object = std::make_unique<TestGameObject>();
		object->Init(index);

		objects_.emplace_back(std::move(object));
	}

	particle_ = std::make_unique<TestParticle>();
	particle_->Init();

	sceneName_ = "Title";

}

void TitleScene::Update() {

	for (const auto& object : objects_) {

		object->Update();
	}

	particle_->Update();

}

void TitleScene::Draw() {

	NewMoonGame::DrawGrid();

	for (const auto& object : objects_) {

		object->Draw();
	}

	particle_->Draw(kBlendModeAdd);

}

void TitleScene::Cleanup() {

	NewMoonGame::ClearAllGameInformation();
}