#include "DXDescriptor.h"

#include "Engine/Base/NewMoon.h"

/*////////////////////////////////////////////////////////////////////////////////
*						DepthStencilTextureを作成する関数
////////////////////////////////////////////////////////////////////////////////*/
ComPtr<ID3D12Resource> DXDescriptor::MakeDepthStencilTextureResource(ID3D12Device* device, int32_t width, int32_t height) {

	HRESULT hr;

	// 生成するResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = width;                                   // Textureの幅
	resourceDesc.Height = height;                                 // Textureの高さ
	resourceDesc.MipLevels = 1;                                   // mipmapの数
	resourceDesc.DepthOrArraySize = 1;                            // 奥行　or 配列Textureの配列数
	resourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;          // DepthStencilとして利用可能なフォーマット
	resourceDesc.SampleDesc.Count = 1;                            // サンプリングカウント。1固定
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;  // 2次元
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL; // DepthStencilとして使う通知

	// 利用するHeapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT; // VRAM上に作る

	// 深度値のクリア設定
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;              // 1.0f(最大値)でクリア
	depthClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // フォーマット、Resourceに合わせる

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

/*////////////////////////////////////////////////////////////////////////////////
*						   DescriptorHeapを作成する関数
////////////////////////////////////////////////////////////////////////////////*/
ComPtr<ID3D12DescriptorHeap> DXDescriptor::MakeDescriptorHeap(
	DescriptorType descriptorType, ID3D12Device* device,
	D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible) {

	HRESULT hr;

	ComPtr<ID3D12DescriptorHeap> descriptorHeap = nullptr;

	descriptorHeapDescs_[static_cast<size_t>(descriptorType)].Type = heapType;
	descriptorHeapDescs_[static_cast<size_t>(descriptorType)].NumDescriptors = numDescriptors;
	descriptorHeapDescs_[static_cast<size_t>(descriptorType)].Flags =
		shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	hr = device->CreateDescriptorHeap(&descriptorHeapDescs_[static_cast<size_t>(descriptorType)], IID_PPV_ARGS(&descriptorHeap));
	assert(SUCCEEDED(hr));

	return descriptorHeap;
}

/*////////////////////////////////////////////////////////////////////////////////
*							Depth Stencil VIewの生成
////////////////////////////////////////////////////////////////////////////////*/
void DXDescriptor::CreateDSV(ID3D12Device* device, uint32_t width, uint32_t height) {

	// DepthStencilResourceの生成
	depthStencilResource_ = MakeDepthStencilTextureResource(device, width, height);

	// DSVの設定
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;        // Format。基本的にはResourceに合わせる
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D; // 2DTexture
	// DSVHeapの先頭に作る
	device->CreateDepthStencilView(depthStencilResource_.Get(),
		&dsvDesc, descriptorHeaps_[static_cast<size_t>(DescriptorType::DSV)]->GetCPUDescriptorHandleForHeapStart());
}

/*////////////////////////////////////////////////////////////////////////////////
*									初期化
////////////////////////////////////////////////////////////////////////////////*/
void DXDescriptor::Init(uint32_t width, uint32_t height) {

	auto device = NewMoon::GetDXDevice();

	descriptorHeaps_[static_cast<size_t>(DescriptorType::DSV)] =
		MakeDescriptorHeap(DescriptorType::DSV, device, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, false);

	// Depth Stencil VIewの生成
	CreateDSV(device, width, height);
}

/*////////////////////////////////////////////////////////////////////////////////
*									Getter
////////////////////////////////////////////////////////////////////////////////*/
ID3D12DescriptorHeap* DXDescriptor::GetDSVDescriptorHeap() const {return descriptorHeaps_[static_cast<size_t>(DescriptorType::DSV)].Get();}

D3D12_CPU_DESCRIPTOR_HANDLE DXDescriptor::GetDSVHandle() {return dsvHandle_;}

ID3D12Resource* DXDescriptor::GetDepthResource() const { return depthStencilResource_.Get(); }
