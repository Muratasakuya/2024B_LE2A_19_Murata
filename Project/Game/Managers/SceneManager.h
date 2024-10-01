#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Engine/Base/NewMoon.h"
#include "Game/Scenes/Methods/IScene.h"
#include "Game/Scenes/Methods/SceneFactory.h"

// c++
#include <memory>

/*////////////////////////////////////////////////////////////////////////////////
*								SceneManager Class
////////////////////////////////////////////////////////////////////////////////*/
class SceneManager {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	SceneManager();
	~SceneManager();

	void Run();

	void ChangeScene(SceneNo sceneNo);

	// singleton
	static SceneManager* GetInstance();

private:
	//===================================================================*/
	//							private Variables
	//===================================================================*/

	std::unique_ptr<IScene> currentScene_;
	SceneFactory sceneFactory_;

	SceneNo currentSceneNo_;
};