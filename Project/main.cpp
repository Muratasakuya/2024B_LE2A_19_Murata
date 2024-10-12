#include "Game/Managers/SceneManager.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	NewMoon::Init(windowWidth, windowHeight);
	NewMoonGame::Init();

	SceneManager* sceneManager = SceneManager::GetInstance();

	sceneManager->Run();

	NewMoonGame::Close();
	NewMoon::Close();

	return 0;
}