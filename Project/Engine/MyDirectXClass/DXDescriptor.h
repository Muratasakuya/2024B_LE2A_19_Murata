#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Engine/MyDirectXClass/ComPtr.h"

// directX
#include <d3d12.h>

// c++
#include <cassert>
#include <array>

// デスクリプタの種類
enum class DescriptorType {

	DSV,
	DescriptorTypeNum
};

// デスクリプタの数
static const uint32_t descriptorNum = static_cast<size_t>(DescriptorType::DescriptorTypeNum);

/*////////////////////////////////////////////////////////////////////////////////
*							DXDescriptor Class
////////////////////////////////////////////////////////////////////////////////*/
class DXDescriptor {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	void CreateDSV(ID3D12Device* device, uint32_t width, uint32_t height);

	void Initialize(uint32_t width, uint32_t height);

	// Getter
	ID3D12DescriptorHeap* GetDSVDescriptorHeap() const;
	D3D12_CPU_DESCRIPTOR_HANDLE GetDSVHandle();
	ID3D12Resource* GetDepthResource() const;

private:
	//===================================================================*/
	//							private Variables
	//===================================================================*/

	std::array<ComPtr<ID3D12DescriptorHeap>, descriptorNum> descriptorHeaps_;
	std::array< D3D12_DESCRIPTOR_HEAP_DESC, descriptorNum> descriptorHeapDescs_;

	// DSV
	ComPtr<ID3D12Resource> depthStencilResource_;
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc_{};
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle_;

private:
	//===================================================================*/
	//							private Functions
	//===================================================================*/

	ComPtr<ID3D12Resource> MakeDepthStencilTextureResource(ID3D12Device* device, int32_t width, int32_t height);

	ComPtr<ID3D12DescriptorHeap> MakeDescriptorHeap(
		DescriptorType descriptorType,
		ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE heapType,
		UINT numDescriptors, bool shaderVisible
	);
};