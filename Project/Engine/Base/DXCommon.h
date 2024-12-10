#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Engine/MyDirectXClass/DXDevice.h"
#include "Engine/MyDirectXClass/DXSwapChain.h"
#include "Engine/MyDirectXClass/DXOffscreenRender.h"
#include "Engine/MyDirectXClass/Pipeline/PipelineStateStructure.h"
#include "Engine/Managers/RtvManager.h"
#include "Engine/Managers/DsvManager.h"

// directX
#include <dxgidebug.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxcapi.h>

// c++
#include <cstdint>
#include <memory>
#include <cassert>
#include <chrono>
#include <thread>

// front
class WinApp;
class SrvManager;

/*////////////////////////////////////////////////////////////////////////////////
*							DirectXCommon Class
////////////////////////////////////////////////////////////////////////////////*/
class DXCommon {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	DXCommon() = default;
	~DXCommon() = default;

	void Init(WinApp* winApp, uint32_t width, uint32_t height);

	void TransitionBarrier(ID3D12Resource* resource, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter);

	void PreDraw();
	void PostDraw();

	void WaitForGPU();

	//* offscreen *//

	void BeginPreOffscreen();

	void EndPostOffscreen();

	void OffscreenDraw(const PipelineType& pipelineType);

	void CreateOffscreenRenderTexture(SrvManager* srvManager, uint32_t width, uint32_t height);

	void Close(WinApp* winApp);

	//* getter *//

	ID3D12Device* GetDevice() const;
	ID3D12GraphicsCommandList* GetCommandList() const;
	IDxcUtils* GetDxcUtils() const;
	IDxcCompiler3* GetDxcCompiler() const;
	IDxcIncludeHandler* GetIncludeHandler() const;
	DXGI_SWAP_CHAIN_DESC1& GetSwapChainDesc();

	ID3D12Resource* GetRenderTexture() const { return renderTextureResource_.Get(); }
	ID3D12Resource* GetGuiRenderTexture() const { return guiRenderTextureResource_.Get(); }
	D3D12_GPU_DESCRIPTOR_HANDLE& GetRendreTextureGpuHandle() { return renderTextureGpuHandle_; }
	D3D12_GPU_DESCRIPTOR_HANDLE& GetGuiRendreTextureGpuHandle() { return guiRenderTextureGpuHandle_; }
	D3D12_GPU_DESCRIPTOR_HANDLE& GetDepthGpuHandle() { return depthGpuHandle_; }

private:
	//===================================================================*/
	//							private Variables
	//===================================================================*/

	HRESULT hr_;
	UINT backBufferIndex_;
	uint64_t fenceValue_;
	HANDLE fenceEvent_;

	std::chrono::steady_clock::time_point reference_;

	//* 画面色
	Vector4 clearColor_;

	//* 画面サイズ
	int32_t kClientWidth_;
	int32_t kClientHeight_;

	std::unique_ptr<DXDevice> device_;
	std::unique_ptr<DXSwapChain> swapChain_;
	std::unique_ptr<DXOffscreenRender> offscreenRender_;
	std::unique_ptr<RtvManager> rtvManager_;
	std::unique_ptr<DsvManager> dsvManager_;

#pragma region /// DXComObject ///
	ComPtr<ID3D12Fence> fence_;
	ComPtr<IDxcUtils> dxcUtils_;
	ComPtr<IDxcCompiler3> dxcCompiler_;
	ComPtr<IDxcIncludeHandler> includeHandler_;
	ComPtr<ID3D12CommandQueue> commandQueue_;
	ComPtr<ID3D12CommandAllocator> commandAllocator_;
	ComPtr<ID3D12GraphicsCommandList> commandList_;
#pragma endregion

#pragma region /// DXObject ///

	//* renderTexture
	ComPtr<ID3D12Resource> renderTextureResource_;
	D3D12_GPU_DESCRIPTOR_HANDLE renderTextureGpuHandle_;

	//* guiRenderTextureResource
	ComPtr<ID3D12Resource> guiRenderTextureResource_;
	D3D12_GPU_DESCRIPTOR_HANDLE guiRenderTextureGpuHandle_;

	//* depth
	D3D12_GPU_DESCRIPTOR_HANDLE depthGpuHandle_;

	D3D12_COMMAND_QUEUE_DESC commandQueueDesc_{};
	D3D12_RESOURCE_BARRIER barrier_{};

	D3D12_VIEWPORT viewport_{};
	D3D12_RECT scissorRect_{};

#pragma endregion

private:
	//===================================================================*/
	//							private Functions
	//===================================================================*/

	void DebugLayer();
	void DebugInfo();

	void CreateFenceEvent();
	void InitDXCompiler();
	void CreateCommand();

	void InitFixFPS();
	void UpdateFixFPS();

	void CopyRenderTexture(ID3D12Resource* dstResource, D3D12_RESOURCE_STATES dstState,
		ID3D12Resource* srcResource, D3D12_RESOURCE_STATES srcState);

};