#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/DXClass/DXSwapChain.h>
#include <Engine/DXClass/DXDevice.h>

// windows
#include <Windows.h>

// directX
#include <dxcapi.h>

// c++
#include <memory>
#include <chrono>

//============================================================================*/
//	DXCommon class
//============================================================================*/
class DXCommon {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	DXCommon() = default;
	~DXCommon() = default;

	void Init(HWND hwnd);

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	std::chrono::steady_clock::time_point reference_;

	std::unique_ptr<DXSwapChain> swapChain_;

	std::unique_ptr<DXDevice> device_;

	ComPtr<ID3D12Fence> fence_;

	ComPtr<IDxcUtils> dxcUtils_;
	ComPtr<IDxcCompiler3> dxcCompiler_;
	ComPtr<IDxcIncludeHandler> includeHandler_;

	ComPtr<ID3D12CommandQueue> commandQueue_;
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc_;

	ComPtr<ID3D12CommandAllocator> commandAllocator_;
	ComPtr<ID3D12GraphicsCommandList> commandList_;

	//========================================================================*/
	//* functions

	void DebugLayer();
	void DebugInfo();

	void CreateFenceEvent();

	void InitDXCompiler();

	void CreateCommand();

};