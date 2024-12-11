#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/DXClass/ComPtr.h>

// driectX
#include <d3d12.h>
#include <dxgi1_6.h>

// c++
#include <array>
#include <cassert>

//============================================================================*/
//	DXSwapChain class
//============================================================================*/
class DXSwapChain {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	DXSwapChain() = default;
	~DXSwapChain() = default;

	void Init(HWND hwnd, IDXGIFactory7* dxgiFactory, ID3D12CommandQueue* commandQueue);

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	// バッファカウント
	static const uint32_t bufferCount_ = 2;

	ComPtr<IDXGISwapChain4> swapChain_;

	std::array<ComPtr<ID3D12Resource>, bufferCount_> resources_;

	DXGI_SWAP_CHAIN_DESC1 desc_;

};