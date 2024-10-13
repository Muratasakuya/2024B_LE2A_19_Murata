#include "SceneManager.h"

#include "Engine/Managers/ImGuiManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*						SceneManager classMethods
////////////////////////////////////////////////////////////////////////////////*/

SceneManager* SceneManager::GetInstance() {
	static SceneManager instance;
	return &instance;
}

void SceneManager::SetTransitioning(bool isTransitioning) {
	// 遷移管理
	isTransitioning_ = isTransitioning;
}

SceneManager::SceneManager() {

	isTransitioning_ = false;

	currentSceneNo_ = GAME;
	currentScene_ = static_cast<std::unique_ptr<IScene>>(sceneFactory_.CreateScene(currentSceneNo_));
	currentScene_->Init();

	transitionScene_ = std::make_unique<TransitionScene>();
	transitionScene_->Init();
}

SceneManager::~SceneManager() {

	currentScene_.reset();
	transitionScene_.reset();
}

void SceneManager::ChangeScene(SceneNo sceneNo, TransitionType transitiontype, float duration) {

	transitionScene_->SetDuration(duration);
	isTransitioning_ = true;

	transitionScene_->Start(transitiontype, [this, sceneNo]() {

		currentSceneNo_ = sceneNo;
		currentScene_ = std::unique_ptr<IScene>(sceneFactory_.CreateScene(currentSceneNo_));
		currentScene_->Init();
		});
}

void SceneManager::Run() {

	while (NewMoon::ProcessMessage() == 0) {

		NewMoon::BeginFrame();
		NewMoonGame::Update();

		if (isTransitioning_) {

			currentScene_->Update();
			currentScene_->Draw();

			transitionScene_->Update();
			transitionScene_->Render();
		} else {

 			currentSceneNo_ = currentScene_->GetSceneNo();

			currentScene_->Update();
			currentScene_->Draw();
		}

		NewMoonGame::Reset();
		NewMoon::EndFrame();
	}

}