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

		BackDraw();
		NewMoon::ClearDepthBuffer();
		FrontDraw();

		NewMoonGame::Reset();
		NewMoon::EndFrame();

		if (SceneManager::GetInstance()->IsSceneSwitching()) {
			break;
		}
	}

	Cleanup();

}

void TitleScene::Init() {



}

void TitleScene::Update() {

	if (NewMoonGame::TriggerKey(DIK_SPACE)) {

		SceneManager::GetInstance()->SetNextScene("Game");
	}
}

void TitleScene::BackDraw() {

}

void TitleScene::FrontDraw() {
}

void TitleScene::Cleanup() {
}