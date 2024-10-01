#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Game/Scenes/Methods/IScene.h"

/*////////////////////////////////////////////////////////////////////////////////
*							AbstractSceneFactory Class
////////////////////////////////////////////////////////////////////////////////*/
class AbstractSceneFactory {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	virtual ~AbstractSceneFactory() = default;

	virtual IScene* CreateScene(SceneNo sceneNo) = 0;
};