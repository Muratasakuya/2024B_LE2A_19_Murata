#pragma once

// Engine
#include "NewMoon.h"

// Base
#include "IScene.h"
// SceneFactory
#include "SceneFactory.h"

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

	SceneNo currentSceneNo_{};
};