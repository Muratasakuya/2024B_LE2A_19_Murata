#include "DsvManager.h"

#include "Engine/Base/NewMoon.h"
#include "Engine/Managers/SrvManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*	DsvManager ClassMethods
////////////////////////////////////////////////////////////////////////////////*/

// SRVの最大数
const uint32_t DsvManager::kMaxDsvCount_ = 8;

void DsvManager::MakeDescriptorHeap() {

	ID3D12Device* device = NewMoon::GetDXDevice();

	desc_.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	desc_.NumDescriptors = kMaxDsvCount_;
	desc_.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	HRESULT hr = device->CreateDescriptorHeap(&desc_, IID_PPV_ARGS(&descriptorHeap_));
	assert(SUCCEEDED(hr));

}

ComPtr<ID3D12Resource> DsvManager::MakeDepthResource(uint32_t width, uint32_t height,
	DXGI_FORMAT resourceFormat, DXGI_FORMAT depthClearFormat) {

	HRESULT hr;
	ID3D12Device* device = NewMoon::GetDXDevice();

	// 生成するResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = width;                                   // Textureの幅
	resourceDesc.Height = height;                                 // Textureの高さ
	resourceDesc.MipLevels = 1;                                   // mipmapの数
	resourceDesc.DepthOrArraySize = 1;                            // 奥行　or 配列Textureの配列数
	resourceDesc.Format = resourceFormat;                         // DepthStencilとして利用可能なフォーマット
	resourceDesc.SampleDesc.Count = 1;                            // サンプリングカウント。1固定
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;  // 2次元
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL; // DepthStencilとして使う通知

	// 利用するHeapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT; // VRAM上に作る

	// 深度値のクリア設定
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;  // 1.0f(最大値)でクリア
	depthClearValue.Format = depthClearFormat; // フォーマット、Resourceに合わせる

	ComPtr<ID3D12Resource> resource = nullptr;
	hr = device->CreateCommittedResource(
		&heapProperties,                  // Heapの設定
		D3D12_HEAP_FLAG_NONE,             // Heapの特殊な設定、特になし
		&resourceDesc,                    // Resourceの設定
		D3D12_RESOURCE_STATE_DEPTH_WRITE, // 深度値を書き込む状態にしておく
		&depthClearValue,                 // Clear最適値
		IID_PPV_ARGS(&resource)           // 作成するResourceポインタへのポインタ
	);
	assert(SUCCEEDED(hr));

	return resource;
}

void DsvManager::CreateDSV(uint32_t width, uint32_t height) {

	ID3D12Device* device = NewMoon::GetDXDevice();

	uint32_t dsvIndex = Allocate();
	dsvCPUHandle_ = GetCPUHandle(dsvIndex);

	resource_ = MakeDepthResource(width, height, DXGI_FORMAT_D24_UNORM_S8_UINT, DXGI_FORMAT_D24_UNORM_S8_UINT);

	// DSVの設定
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;

	device->CreateDepthStencilView(resource_.Get(), &dsvDesc, dsvCPUHandle_);

}

void DsvManager::Init(uint32_t width, uint32_t height) {

	ID3D12Device* device = NewMoon::GetDXDevice();

	useIndex_ = 0;
	descriptorSize_ = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	// descripptorHeapの作成
	MakeDescriptorHeap();
	// DSVの作成
	CreateDSV(width, height);

}

uint32_t DsvManager::Allocate() {

	if (!CanAllocate()) {
		throw std::runtime_error("Cannot allocate more DSVs, maximum count reached.");
	}

	// returnする番号を一旦記録しておく
	int index = useIndex_;
	// 次回のために1進める
	useIndex_++;

	return index;
}

bool DsvManager::CanAllocate() {
	return useIndex_ < kMaxDsvCount_;
}

D3D12_CPU_DESCRIPTOR_HANDLE DsvManager::GetCPUHandle(uint32_t index) {

	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap_.Get()->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize_ * index);

	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE DsvManager::GetGPUHandle(uint32_t index) {

	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap_.Get()->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize_ * index);

	return handleGPU;
}