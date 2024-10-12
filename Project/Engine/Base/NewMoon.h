#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Engine/Managers/PipelineManager.h"
#include "Game/Components/MaterialObject.h"
#include "Lib/Structure.h"

// c++
#include <cstdint>
#include <memory>
#include <array>
#include <iostream>

///===============================================================================
/// クラス前方宣言
#pragma region
class WinApp;
class DXCommon;
class SrvManager;
class ImGuiManager;
#pragma endregion
///===============================================================================

/*////////////////////////////////////////////////////////////////////////////////
*									NewMoon Class
////////////////////////////////////////////////////////////////////////////////*/
class NewMoon {
private:
	//===================================================================*/
	//							private Method
	//===================================================================*/

	struct LeakChecker {

		~LeakChecker() {

			ComPtr<IDXGIDebug1> debug;
			if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug)))) {

				debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
				debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
				debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
			}
		}
	};
	LeakChecker leakCheck;

public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	NewMoon() = default;
	~NewMoon() = default;

	///===================================================================
	// MainSystem

	static void Init(uint32_t width, uint32_t height);       // 各システムの初期化
	static void BeginFrame();                                // フレーム開始処理
	static void EndFrame();                                  // フレーム終了処理
	static void OffscreenPreDraw();                          // オフスクリーン描画前処理
	static void OffscreenPostDraw();                         // オフスクリーン描画後処理
	static void PreDraw();                                   // 描画前処理
	static void PostDraw();                                  // 描画後処理
	static void Close();                                     // システムの終了
	static bool ProcessMessage();                            // メッセージの受け渡し処理

	///===================================================================
	// TestOffscreen

	static void OffscreenDraw();
	static void OffscreenDepthOutlineDraw(OffscreenDepthMaterial& depthMaterial);
	static void OffscreenDissolveDraw(OffscreenDissolveMaterial& dissolveMaterial);

	///===============================================================================
	// Screen

	static void SetFullScreenMode(bool fullScreen); // フルスクリーン設定
	static bool IsFullScreenMode();                 // フルスクリーンかどうか

	///===============================================================================
	// Command

	static void SetGraphicsRootDescriptorTable(ID3D12GraphicsCommandList* commandList, UINT rootParamaterIndex, std::string identifier);
	static void SetGraphicsPipeline(ID3D12GraphicsCommandList* commandList, PipelineType pipelineType, BlendMode blendMode);
	static void SetComputePipeline(ID3D12GraphicsCommandList* commandList, ComputePipelineType csPipelineType);
	static void TransitionBarrier(ID3D12Resource* resource, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter);
	static void ClearDepthBuffer();

	///===============================================================================
	// Getter

	static SrvManager* GetSrvManagerPtr();
	static DXCommon* GetDXCommonPtr();
	static WinApp* GetWinAppPtr();
	static ID3D12Device* GetDXDevice();
	static ID3D12GraphicsCommandList* GetCommandList();

public:
	//===================================================================*/
	//							public Variables
	//===================================================================*/

	// ウィンドウサイズ
	// int
	static uint32_t kWindowWidthd;
	static uint32_t kWindowHeightd;
	// float
	static float kWindowWidthf;
	static float kWindowHeightf;

	static Vector2 kWindowSizef;

private:
	//===================================================================*/
	//							private Variables
	//===================================================================*/

	static std::unique_ptr<WinApp> winApp_;
	static std::unique_ptr<DXCommon> dxCommon_;

	static std::unique_ptr<SrvManager> srvManager_;
	static std::unique_ptr<PipelineManager> pipelineManager_;

	static std::unique_ptr<ImGuiManager> imguiManager_;

};