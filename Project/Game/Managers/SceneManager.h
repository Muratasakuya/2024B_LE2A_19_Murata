#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Engine/Base/NewMoon.h"
#include "Engine/Base/NewMoonGame.h"
#include "Engine/Methods/ScreenDimensions.h"
#include "Game/Scenes/Methods/IScene.h"
#include "Game/Scenes/Methods/SceneFactory.h"
#include "Game/Scenes/Methods/TransitionScene.h"

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

	void ChangeScene(SceneNo sceneNo, TransitionType transitiontype, float duration = 1.0f);

	static SceneManager* GetInstance();

	// Setter
	void SetTransitioning(bool isTransitioning);

private:
	//===================================================================*/
	//							private Variables
	//===================================================================*/

	std::unique_ptr<IScene> currentScene_;
	std::unique_ptr<TransitionScene> transitionScene_;
	SceneFactory sceneFactory_;

	SceneNo currentSceneNo_;
	bool isTransitioning_;
};