#include "SceneFactory.h"

#include "Game/Scenes/GameScene.h"
#include "Game/Scenes/TitleScene.h"

/*////////////////////////////////////////////////////////////////////////////////
*						SceneFactory classMethod
////////////////////////////////////////////////////////////////////////////////*/

std::shared_ptr<IScene> SceneFactory::CreateScene(const std::string& sceneName) {

	if (sceneName == "Game") {

		return std::make_shared<GameScene>();
	} else if (sceneName == "Title") {

		return std::make_shared<TitleScene>();
	}

	assert(false && "input scene Name does not Create Methods");
	return nullptr;
}