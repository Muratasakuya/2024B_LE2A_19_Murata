#include "NewMoon.h"

//============================================================================*/
//	NewMoon classMethods
//============================================================================*/

std::unique_ptr<WinApp> NewMoon::winApp_ = nullptr;

void NewMoon::Init() {
#pragma warning(push)
#pragma warning(disable:6031)
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);
#pragma warning(pop)

	winApp_ = std::make_unique<WinApp>();
	winApp_->Init();

}

void NewMoon::Finalize() {
}

bool NewMoon::ProcessMessage() {
	return winApp_->ProcessMessage();
}