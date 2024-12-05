#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Engine/Methods/ScreenDimensions.h"
#include "Game/Scenes/Methods/IScene.h"
#include "Game/Scenes/Methods/SceneFactory.h"

// c++
#include <memory>
#include <string>
#include <map>
#include <future>

/*////////////////////////////////////////////////////////////////////////////////
*								SceneManager Class
////////////////////////////////////////////////////////////////////////////////*/
class SceneManager {
public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	SceneManager();
	~SceneManager();

	//* Loop *//

	void Run();

	void SetNextScene(const std::string& sceneName);

	static SceneManager* GetInstance();

	void ImGui();

	// Getter
	bool IsSceneSwitching() const;

private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

	std::unique_ptr<IScene> currentScene_;

	std::map<std::string, std::shared_ptr<IScene>> loadedScenes_;

	std::string nextSceneName_;
	bool isSceneSwitching_;
	bool gameLoop_;

	int selectedSceneIndex_;

private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

	//* LoadMethods *//

	void LoadScene(const std::string& sceneName);

};