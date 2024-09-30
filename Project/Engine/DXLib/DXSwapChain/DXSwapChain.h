#pragma once
#pragma once

// DirectX
#include <d3d12.h>
#include <dxgi1_6.h>

#include "ComPtr.h"

// c++
#include <cstdint>
#include <cassert>

///===============================================================================
/// クラス前方宣言
class WinApp;
///===============================================================================

/*////////////////////////////////////////////////////////////////////////////////
*							DXSwapChain Class
////////////////////////////////////////////////////////////////////////////////*/
class DXSwapChain {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	// バッファカウント
	static const uint32_t bufferCount = 2;

	DXSwapChain() = default;
	~DXSwapChain() = default;

	void Initialize(
		WinApp* winApp, IDXGIFactory7* dxgiFactory, ID3D12CommandQueue* commandQueue,
		uint32_t width, uint32_t height);

	// Getter
	IDXGISwapChain4* Get() const;
	ID3D12Resource* GetResources(uint32_t num) const;
	DXGI_SWAP_CHAIN_DESC1& GetDesc();

private:
	//===================================================================*/
	//							private Variables
	//===================================================================*/

	ComPtr<IDXGISwapChain4> swapChain_;
	ComPtr<ID3D12Resource> swapChainResources_[bufferCount];
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc_{};
};