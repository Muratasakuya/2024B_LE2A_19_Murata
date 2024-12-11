#include "MeshRenderer.h"

#include <imgui.h>

/*////////////////////////////////////////////////////////////////////////////////
*	MeshRenderer ClassMethods
////////////////////////////////////////////////////////////////////////////////*/

std::vector<IBaseGameObject*> MeshRenderer::gameObjects_ = {};
IBaseGameObject* MeshRenderer::selectedGameObject_ = nullptr;
int MeshRenderer::currentObjectIndex_ = 0;

void MeshRenderer::Render() {

	for (const auto& gameObject : gameObjects_) {

		gameObject->Draw();
	}
}

void MeshRenderer::SetGameObject(IBaseGameObject* gameObject) {

	if (!gameObject) {
		return;
	}

	gameObjects_.emplace_back(gameObject);
}

void MeshRenderer::EraseGameObject(IBaseGameObject* gameObject) {

	gameObjects_.erase(std::remove(gameObjects_.begin(), gameObjects_.end(), gameObject), gameObjects_.end());
}

void MeshRenderer::Clear() {

	gameObjects_.clear();
}