#include "TitleScene.h"

#include "Engine/Base/NewMoon.h"
#include "Engine/Base/NewMoonGame.h"
#include "Engine/Managers/ImGuiManager.h"
#include "Game/Managers/SceneManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*							TitleScene classMethods
////////////////////////////////////////////////////////////////////////////////*/

TitleScene::TitleScene() {}
TitleScene::~TitleScene() {}

void TitleScene::Run() {

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


void TitleScene::Load() {

	NewMoonGame::LoadModel(baseModelDirectory_, "cube.obj");

}

void TitleScene::Init() {

	Load();

	particle_ = std::make_unique<TestParticle>();
	particle_->Init();

}

void TitleScene::Update() {

	particle_->Update();

}

void TitleScene::Draw() {

	NewMoonGame::DrawGrid();

	particle_->Draw(kBlendModeAdd);

}

void TitleScene::Cleanup() {
}