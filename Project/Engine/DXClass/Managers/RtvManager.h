#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/DXClass/ComPtr.h>

// directX
#include <d3d12.h>

// c++
#include <cstdint>

//============================================================================*/
//	RtvManager class
//============================================================================*/
class RtvManager {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	RtvManager() = default;
	~RtvManager() = default;

	void Init();

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	static const uint32_t kMaxRtvCount_;
	uint32_t useIndex_;

	ComPtr<ID3D12DescriptorHeap> descriptorHeap_;
	D3D12_RENDER_TARGET_VIEW_DESC desc_;

	uint32_t descriptorSize_;

	//========================================================================*/
	//* functions

	ComPtr<ID3D12DescriptorHeap> MakeDescriptorHeap(ID3D12Device* device);

};