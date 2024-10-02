#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Game/Scenes/Methods/AbstractSceneFactory.h"

/*////////////////////////////////////////////////////////////////////////////////
*								IScene Class
////////////////////////////////////////////////////////////////////////////////*/
class SceneFactory :
	public AbstractSceneFactory {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	IScene* CreateScene(SceneNo sceneNo) override;

};