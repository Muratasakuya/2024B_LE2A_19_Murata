#pragma once

// Windows
#include <Windows.h>

// Lib
#include "Logger.h"

/*////////////////////////////////////////////////////////////////////////////////
*							Windows Application Class 
////////////////////////////////////////////////////////////////////////////////*/
class WinApp {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	WinApp() = default;
	~WinApp() = default;

	bool ProcessMessage();
	void CreateMainWindow(uint32_t width, uint32_t height);

	void SetFullscreen(bool fullscreen);

	// Getter
	HWND GetHwnd() const;
	WNDCLASS GetWindowClass() const;
	bool IsFullscreen() const;

private:
	//===================================================================*/
	//							private Variables
	//===================================================================*/

	// windowHandle
	HWND hwnd_{};
	WNDCLASS wc_{};

	bool isFullscreen_ = false;
	UINT windowStyle_;
	RECT windowRect_;

private:
	//===================================================================*/
	//							private Functions
	//===================================================================*/

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	void RegisterWindowClass();
};