
//===================================================================*/
//								include
//===================================================================*/
#include "Engine/Base/NewMoon.h"
#include "Engine/Base/NewMoonGame.h"
#include "Game/Managers/SceneManager.h"
#include <Windows.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	NewMoon::Init(windowWidth, windowHeight);
	NewMoonGame::Init();

	SceneManager* sceneManager = SceneManager::GetInstance();

	sceneManager->Run();

	NewMoonGame::Close();
	NewMoon::Close();

	return 0;
}