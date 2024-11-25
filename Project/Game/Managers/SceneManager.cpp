#include "SceneManager.h"

#include "Engine/Base/NewMoon.h"
#include "Engine/Managers/ImGuiManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*						SceneManager classMethods
////////////////////////////////////////////////////////////////////////////////*/

SceneManager* SceneManager::GetInstance() {

	static SceneManager instance;
	return &instance;
}

bool SceneManager::IsSceneSwitching() const {
	return isSceneSwitching_;
}

SceneManager::SceneManager() {

	isSceneSwitching_ = false;
	gameLoop_ = true;

	LoadScene("Title");
}
SceneManager::~SceneManager() {}

void SceneManager::Run() {

	while (gameLoop_) {
		if (currentScene_) {

			currentScene_->Run();
		}

		if (NewMoon::ProcessMessage() == 0 && !isSceneSwitching_) {
			gameLoop_ = false;
		}

		if (isSceneSwitching_) {

			// Load
			LoadScene(nextSceneName_);

			isSceneSwitching_ = false;
		}
	}
}

void SceneManager::SetNextScene(const std::string& sceneName) {

	nextSceneName_ = sceneName;
	isSceneSwitching_ = true;
}

void SceneManager::LoadScene(const std::string& sceneName) {

	auto it = loadedScenes_.find(sceneName);
	if (it != loadedScenes_.end()) {

		currentScene_ = it->second;
		currentScene_->Init();
		return;
	}

	//* CreateNewScene *//
	std::shared_ptr<IScene> scene = SceneFactory::CreateScene(sceneName);
	if (scene) {

		currentScene_ = scene;
		loadedScenes_[sceneName] = scene;
	}
}