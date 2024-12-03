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

void SceneManager::ImGui() {
#ifdef _DEBUG

	ImGui::SetCursorPos(ImVec2(397.0f, 64.0f + 432.0f + 4.0f));
	ImGui::BeginChild("SceneChild",
		ImVec2((((NewMoon::kWindowWidthf / 2.0f) + (768.0f / 2.0f) - 254.0f)) / 2.0f, 212.0f),
		true, ImGuiWindowFlags_AlwaysUseWindowPadding);

	ImGui::Text("Scene");
	ImGui::Separator();
	std::string currentSceneText = "CurrentScene: " + currentScene_->GetSceneName();
	ImGui::Text("%s", currentSceneText.c_str());

	std::vector<std::string> sceneList = { "Title", "Game" };
	sceneList.erase(std::remove(sceneList.begin(), sceneList.end(), currentScene_->GetSceneName()), sceneList.end());

	ImGui::SetNextItemWidth(162.0f);
	if (ImGui::BeginCombo("TransitionTarget", sceneList[selectedSceneIndex_].c_str(), ImGuiComboFlags_NoArrowButton)) {
		for (int i = 0; i < sceneList.size(); ++i) {
			bool isSelected = (selectedSceneIndex_ == i);
			if (ImGui::Selectable(sceneList[i].c_str(), isSelected)) {

				selectedSceneIndex_ = i;

				SetNextScene(sceneList[selectedSceneIndex_]);

			}
			if (isSelected) {

				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	//currentScene_->GetTransition()->ImGui();

	ImGui::EndChild();

#endif // _DEBUG
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

	currentScene_.reset();

	//* CreateNewScene *//
	std::unique_ptr<IScene> scene = SceneFactory::CreateScene(sceneName);

	currentScene_ = std::move(scene);
}