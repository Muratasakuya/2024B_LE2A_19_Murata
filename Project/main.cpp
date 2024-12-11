
//===================================================================*/
//	include
//===================================================================*/
#include <Engine/Base/NewMoon.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	NewMoon::Init();

	while (!NewMoon::ProcessMessage()) {

		int a = 0;
		a = 1;
	}

	return 0;
}