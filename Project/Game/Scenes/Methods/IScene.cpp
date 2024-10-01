#include "IScene.h"

// ゲームシーンで初期化
SceneNo IScene::sceneNo_ = SceneNo::GAME;

SceneNo IScene::GetSceneNo() const { return sceneNo_; }