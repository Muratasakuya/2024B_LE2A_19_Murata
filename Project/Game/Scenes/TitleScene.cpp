#include "TitleScene.h"

#include "Engine/Managers/ImGuiManager.h"
#include "Game/Managers/SceneManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*						TitleScene classMethods
////////////////////////////////////////////////////////////////////////////////*/

TitleScene::TitleScene() {}
TitleScene::~TitleScene() {}

void TitleScene::Init() {
}

void TitleScene::Update() {

	if (NewMoonGame::TriggerKey(DIK_SPACE)) {

		SceneManager::GetInstance()->ChangeScene(GAME, TransitionType::Fade);
	}
}

void TitleScene::Draw() {
}