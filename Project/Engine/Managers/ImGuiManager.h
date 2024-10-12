#pragma once

//===================================================================*/
//								include
//===================================================================*/
// imgui
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>

// c++
#include <cassert>

///===============================================================================
// 前方宣言
class WinApp;
class DXCommon;
class SrvManager;
///===============================================================================

/*////////////////////////////////////////////////////////////////////////////////
*							ImGuiManager Class
////////////////////////////////////////////////////////////////////////////////*/
class ImGuiManager {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	ImGuiManager() = default;
	~ImGuiManager() = default;

	void Init(WinApp* winApp,DXCommon* dxCommon, SrvManager* srvManager);

	void Begin();
	void End();
	void Draw();

	void Close();

private:
	//===================================================================*/
	//							private Variables
	//===================================================================*/

	WinApp* winApp_ = nullptr;
	DXCommon* dxCommon_ = nullptr;
	SrvManager* srvManager_ = nullptr;

};