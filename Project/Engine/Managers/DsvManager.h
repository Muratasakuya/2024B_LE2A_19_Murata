#pragma once

//===================================================================*/
//	include
//===================================================================*/
#include "Engine/MyDirectXClass/ComPtr.h"

// directX
#include <d3d12.h>

// c++
#include <cstdint>
#include <array>
#include <cassert>

// 一回普通のシャドウマップを実装する

// ShadowCascadeの数
//static const constexpr uint32_t numCascades = 4;

/*////////////////////////////////////////////////////////////////////////////////
*	DsvManager Class
////////////////////////////////////////////////////////////////////////////////*/
class DsvManager {
public:
	//===================================================================*/
	//	public Methods
	//===================================================================*/

	DsvManager() = default;
	~DsvManager() = default;

	void Init(uint32_t width, uint32_t height);

	uint32_t Allocate();
	bool CanAllocate();

	//* getter *//

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle(uint32_t index);

	ID3D12DescriptorHeap* GetDescriptorHeap() const { return descriptorHeap_.Get(); };

	D3D12_CPU_DESCRIPTOR_HANDLE& GetNoramlCPUHandle() { return dsvCPUHandle_; }

	ID3D12Resource* GetDepthResource() const { return resource_.Get(); }

private:
	//===================================================================*/
	//	private Methods
	//===================================================================*/

	//===================================================================*/
	///* variables

	static const uint32_t kMaxDsvCount_;

	uint32_t descriptorSize_;

	uint32_t useIndex_;

	D3D12_DESCRIPTOR_HEAP_DESC desc_;
	ComPtr<ID3D12DescriptorHeap> descriptorHeap_;

	ComPtr<ID3D12Resource> resource_;
	D3D12_CPU_DESCRIPTOR_HANDLE dsvCPUHandle_;

	//===================================================================*/
	///* functions

	void MakeDescriptorHeap();
	void CreateDSV(uint32_t width, uint32_t height);

	ComPtr<ID3D12Resource> MakeDepthResource(uint32_t width, uint32_t height,
		DXGI_FORMAT resourceFormat, DXGI_FORMAT depthClearFormat);

};