#include "SceneManager.h"

#include "Engine/Managers/ImGuiManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*						SceneManager classMethods
////////////////////////////////////////////////////////////////////////////////*/

// 悪魔
SceneManager* SceneManager::GetInstance() {

	static SceneManager instance;
	return &instance;
}

SceneManager::SceneManager() {

	currentSceneNo_ = GAME;
	currentScene_ = static_cast<std::unique_ptr<IScene>>(sceneFactory_.CreateScene(currentSceneNo_));
	currentScene_->Init();
}

SceneManager::~SceneManager() {
	currentScene_.reset();
}

void SceneManager::ChangeScene(SceneNo sceneNo) {

	currentSceneNo_ = sceneNo;
	currentScene_ = static_cast<std::unique_ptr<IScene>>(sceneFactory_.CreateScene(currentSceneNo_));
	currentScene_->Init();
}

void SceneManager::Run() {

	while (NewMoon::ProcessMessage() == 0) {

		NewMoon::BeginFrame();
		NewMoonGame::Update();

		currentSceneNo_ = currentScene_->GetSceneNo();

		currentScene_->Update();
		currentScene_->Draw();

		NewMoonGame::Reset();
		NewMoon::EndFrame();
	}

}