#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Engine/MyDirectXClass/ComPtr.h"

// directX
#include <d3d12.h>

// c++
#include <cstdint>
#include <vector>
#include <string>
#include <utility>
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

	uint32_t Allocate(const std::string& name);
	bool CanAllocate();

	void CreateSRVForTexture2D(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT format, UINT mipLevels);
	void CreateSRVForDepthTexture2D(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT format, UINT mipLevels);
	void CreateSRVForStructureBuffer(uint32_t srvIndex, ID3D12Resource* pResource, UINT numElements, UINT structureByteStride);
	void CreateUAVForStructureBuffer(uint32_t uavIndex, ID3D12Resource* pResource, UINT numElements, UINT structureByteStride);

	void PreDraw();

	void Init();

	void ImGui();

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle(uint32_t index);

	// Getter
	ID3D12DescriptorHeap* GetDescriptorHeap() const;

private:
	//===================================================================*/
	//							private Variables
	//===================================================================*/

	static const uint32_t kMaxSRVCount_;
	std::vector<std::pair<uint32_t, std::string>> srvUseInformations_;

	ComPtr<ID3D12DescriptorHeap> descriptorHeap_;
	uint32_t descriptorSize_;

	bool showSrvList_;

private:
	//===================================================================*/
	//							private Function
	//===================================================================*/

	ComPtr<ID3D12DescriptorHeap> MakeDescriptorHeap(ID3D12Device* device);
};