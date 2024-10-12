#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Engine/Base/NewMoon.h"
#include "Engine/Base/NewMoonGame.h"
#include "Engine/Methods/ScreenDimensions.h"
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

	// 悪魔、c++の汚点
	static SceneManager* GetInstance();

private:
	//===================================================================*/
	//							private Variables
	//===================================================================*/

	std::unique_ptr<IScene> currentScene_;
	SceneFactory sceneFactory_;

	SceneNo currentSceneNo_;
};