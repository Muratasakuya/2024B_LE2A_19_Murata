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
	NewMoonGame::LoadTexture("circle");
	NewMoonGame::LoadTexture("noise");

	NewMoonGame::LoadModel(baseModelDirectory_, "cube.obj");
	NewMoonGame::LoadModel(baseModelDirectory_, "sphere.obj");
	NewMoonGame::LoadModel(baseModelDirectory_, "plane.obj");
	NewMoonGame::LoadModel(baseModelDirectory_, "teapot.obj");

}

void TitleScene::Init() {

	Load();

	player_ = std::make_unique<Player>();
	player_->Init();

	staticMesh_ = std::make_unique<StaticMeshModel>();
	staticMesh_->Init();

	NewMoonGame::SetToStaticMeshImGui(staticMesh_.get());

	field_ = std::make_unique<TemplateField>();
	field_->Init();

	fieldParticle_ = std::make_unique<FieldParticle>();
	fieldParticle_->Init();
	
	injectionParticle_ = std::make_unique<InjectionParticle>();
	injectionParticle_->Init();

	dispersionParticle = std::make_unique<DispersionParticle>();
	dispersionParticle->Init();

	NewMoonGame::GameCamera()->GetFollowCamera()->Init(&player_->GetWorldTransform());

	sceneName_ = "Title";

}

void TitleScene::Update() {

	field_->Update();

	player_->Update();

	staticMesh_->Update();

	fieldParticle_->Update();

	injectionParticle_->Update();

	dispersionParticle->Update();

}

void TitleScene::Draw() {

	field_->Draw();

	player_->Draw();

	staticMesh_->Draw();

	fieldParticle_->Draw(kBlendModeAdd);

	injectionParticle_->Draw(kBlendModeAdd);

	dispersionParticle->Draw(kBlendModeAdd);

}

void TitleScene::Cleanup() {

	NewMoonGame::ClearAllGameInformation();
}