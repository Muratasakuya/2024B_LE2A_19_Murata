#include "SceneFactory.h"

#include "Game/Scenes/GameScene.h"
#include "Game/Scenes/TitleScene.h"
#include "Game/Scenes/TutorialScene.h"
#include "Game/Scenes/SelectScene.h"

/*////////////////////////////////////////////////////////////////////////////////
*								シーン生成
////////////////////////////////////////////////////////////////////////////////*/
IScene* SceneFactory::CreateScene(SceneNo sceneNo) {

	// シーン遷移
	switch (sceneNo) {

	case TITLE:

		return new TitleScene();
	case TUTORIAL:

		return new TutorialScene();
	case SELECT:

		return new SelectScene();
	case GAME:

		return new GameScene();
	}

	return nullptr;
}