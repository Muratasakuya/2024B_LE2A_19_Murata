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

		NewMoon::OffscreenDraw(PipelineType::OffscreenSmoothing);

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
	NewMoonGame::LoadTexture("circle");
	NewMoonGame::LoadTexture("noise");

	NewMoonGame::LoadModel(baseModelDirectory_, "cube.obj");
	NewMoonGame::LoadModel(baseModelDirectory_, "sphere.obj");
	NewMoonGame::LoadModel(baseModelDirectory_, "plane.obj");

}

void TitleScene::Init() {

	Load();

	player_ = std::make_unique<Player>();
	player_->Init();

	field_ = std::make_unique<TemplateField>();
	field_->Init();

	fieldParticle_ = std::make_unique<FieldParticle>();
	fieldParticle_->Init();

	dispersionParticle = std::make_unique<DispersionParticle>();
	dispersionParticle->Init();

	NewMoonGame::GameCamera()->GetFollowCamera()->Init(&player_->GetWorldTransform());

	sceneName_ = "Title";

}

void TitleScene::Update() {

	player_->Update();

	field_->Update();

	fieldParticle_->Update();

	dispersionParticle->Update();

}

void TitleScene::Draw() {

	field_->Draw();

	player_->Draw();

	fieldParticle_->Draw(kBlendModeAdd);

	dispersionParticle->Draw(kBlendModeAdd);

}

void TitleScene::Cleanup() {

	NewMoonGame::ClearAllGameInformation();
}