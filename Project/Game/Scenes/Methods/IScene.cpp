#include "IScene.h"

std::string IScene::sceneName_ = "";

std::unique_ptr<Transition> IScene::transitionScene_ = nullptr;