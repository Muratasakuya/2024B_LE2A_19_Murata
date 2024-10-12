#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Engine/MyDirectXClass/ComPtr.h"

// directX
#include <d3d12.h>

// c++
#include <cstdint>
#include <cassert>

/*////////////////////////////////////////////////////////////////////////////////
*							RtvManager Class
////////////////////////////////////////////////////////////////////////////////*/
class RtvManager {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	RtvManager() = default;
	~RtvManager() = default;

	uint32_t Allocate();
	bool CanAllocate();

	void Create(uint32_t index, ID3D12Resource* resource);

	void Init();

	void Reset();

	// Getter
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(uint32_t index);
	D3D12_RENDER_TARGET_VIEW_DESC& GetDesc();
	ID3D12DescriptorHeap* GetDescriptorHeap() const;

private:
	//===================================================================*/
	//							private Variables
	//===================================================================*/

	static const uint32_t kMaxRtvCount_;
	uint32_t useIndex_;

	D3D12_RENDER_TARGET_VIEW_DESC desc_;
	ComPtr<ID3D12DescriptorHeap> descriptorHeap_;
	uint32_t descriptorSize_;

private:
	//===================================================================*/
	//							private Function
	//===================================================================*/

	ComPtr<ID3D12DescriptorHeap> MakeDescriptorHeap(ID3D12Device* device);

};

