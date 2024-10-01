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
*							SrvManager Class
////////////////////////////////////////////////////////////////////////////////*/
class SrvManager {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	SrvManager() = default;
	~SrvManager() = default;

	uint32_t Allocate();
	bool CanAllocate();

	void CreateSRVForTexture2D(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT format, UINT mipLevels);
	void CreateSRVForDepthTexture2D(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT format, UINT mipLevels);
	void CreateSRVForStructureBuffer(uint32_t srvIndex, ID3D12Resource* pResource, UINT numElements, UINT structureByteStride);
	void CreateUAVForStructureBuffer(uint32_t uavIndex, ID3D12Resource* pResource, UINT numElements, UINT structureByteStride);

	void PreDraw();

	void Initialize();

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle(uint32_t index);

	// Getter
	ID3D12DescriptorHeap* GetDescriptorHeap() const;

private:
	//===================================================================*/
	//							private Variables
	//===================================================================*/

	static const uint32_t kMaxSRVCount_;
	uint32_t useIndex_ = 0;

	ComPtr<ID3D12DescriptorHeap> descriptorHeap_;
	uint32_t descriptorSize_;

private:
	//===================================================================*/
	//							private Function
	//===================================================================*/

	ComPtr<ID3D12DescriptorHeap> MakeDescriptorHeap(ID3D12Device* device);
};