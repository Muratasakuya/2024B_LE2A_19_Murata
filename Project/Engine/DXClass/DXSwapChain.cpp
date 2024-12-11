#include "DXSwapChain.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Utility/Environment.h>

//============================================================================*/
//	DXSwapChain classMethods
//============================================================================*/

void DXSwapChain::Init(HWND hwnd, IDXGIFactory7* dxgiFactory, ID3D12CommandQueue* commandQueue) {

	swapChain_ = nullptr;
	desc_.Width = kWindowWidth;
	desc_.Height = kWindowHeight;
	desc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc_.SampleDesc.Count = 1;
	desc_.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc_.BufferCount = bufferCount_;                    // ダブルバッファ
	desc_.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

	HRESULT hr = dxgiFactory->CreateSwapChainForHwnd(
		commandQueue, hwnd, &desc_, nullptr, nullptr,
		reinterpret_cast<IDXGISwapChain1**>(swapChain_.GetAddressOf()));
	assert(SUCCEEDED(hr));

	hr = swapChain_->GetBuffer(0, IID_PPV_ARGS(&resources_[0]));
	assert(SUCCEEDED(hr));

	hr = swapChain_->GetBuffer(1, IID_PPV_ARGS(&resources_[1]));
	assert(SUCCEEDED(hr));

}