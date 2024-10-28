#include "SrvManager.h"

#include "Engine/Base/NewMoon.h"
#include "Engine/Managers/ImGuiManager.h"

// SRVの最大数
const uint32_t SrvManager::kMaxSRVCount_ = 512;

// DescriptorHeapを作成する関数
ComPtr<ID3D12DescriptorHeap> SrvManager::MakeDescriptorHeap(ID3D12Device* device) {

	HRESULT hr;

	ComPtr<ID3D12DescriptorHeap> descriptorHeap = nullptr;

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.NumDescriptors = kMaxSRVCount_;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	hr = device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&descriptorHeap));
	assert(SUCCEEDED(hr));

	return descriptorHeap;
}

/*////////////////////////////////////////////////////////////////////////////////
*							SRV生成 2DTexture
////////////////////////////////////////////////////////////////////////////////*/
void SrvManager::CreateSRVForTexture2D(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT format, UINT mipLevels) {

	auto device = NewMoon::GetDXDevice();

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = mipLevels;

	device->CreateShaderResourceView(pResource, &srvDesc, GetCPUHandle(srvIndex));
}

/*////////////////////////////////////////////////////////////////////////////////
*							SRV生成 Depth2DTexture
////////////////////////////////////////////////////////////////////////////////*/
void SrvManager::CreateSRVForDepthTexture2D(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT format, UINT mipLevels) {

	auto device = NewMoon::GetDXDevice();

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = mipLevels;

	device->CreateShaderResourceView(pResource, &srvDesc, GetCPUHandle(srvIndex));
}

/*////////////////////////////////////////////////////////////////////////////////
*						  SRV生成 StructureBuffer
////////////////////////////////////////////////////////////////////////////////*/
void SrvManager::CreateSRVForStructureBuffer(uint32_t srvIndex, ID3D12Resource* pResource, UINT numElements, UINT structureByteStride) {

	auto device = NewMoon::GetDXDevice();

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	srvDesc.Buffer.NumElements = numElements;
	srvDesc.Buffer.StructureByteStride = structureByteStride;

	device->CreateShaderResourceView(pResource, &srvDesc, GetCPUHandle(srvIndex));
}

/*////////////////////////////////////////////////////////////////////////////////
*						  UAV生成 StructureBuffer
////////////////////////////////////////////////////////////////////////////////*/
void SrvManager::CreateUAVForStructureBuffer(uint32_t uavIndex, ID3D12Resource* pResource, UINT numElements, UINT structureByteStride) {

	auto device = NewMoon::GetDXDevice();

	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc{};
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.CounterOffsetInBytes = 0;
	uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
	uavDesc.Buffer.NumElements = numElements;
	uavDesc.Buffer.StructureByteStride = structureByteStride;

	device->CreateUnorderedAccessView(pResource, nullptr, &uavDesc, GetCPUHandle(uavIndex));
}

/*////////////////////////////////////////////////////////////////////////////////
*							使用インデックスの計算
////////////////////////////////////////////////////////////////////////////////*/
uint32_t SrvManager::Allocate() {

	if (!CanAllocate()) {
		throw std::runtime_error("Cannot allocate more SRVs, maximum count reached.");
	}

	// returnする番号を一旦記録しておく
	int index = useIndex_;
	// 次回のために1進める
	useIndex_++;

	return index + 1;
}

/*////////////////////////////////////////////////////////////////////////////////
*							割り当て可能かどうかのチェック
////////////////////////////////////////////////////////////////////////////////*/
bool SrvManager::CanAllocate() { return useIndex_ < kMaxSRVCount_; }

/*////////////////////////////////////////////////////////////////////////////////
*								  描画前処理
////////////////////////////////////////////////////////////////////////////////*/
void SrvManager::PreDraw() {

	auto commandList = NewMoon::GetCommandList();

	ID3D12DescriptorHeap* descriptorHeaps[] = { descriptorHeap_.Get() };
	commandList->SetDescriptorHeaps(1, descriptorHeaps);
}

/*////////////////////////////////////////////////////////////////////////////////
*									初期化
////////////////////////////////////////////////////////////////////////////////*/
void SrvManager::Init() {

	auto device = NewMoon::GetDXDevice();

	// 生成
	descriptorHeap_ =
		MakeDescriptorHeap(device);
	// デスクリプタ1個のサイズの取得
	descriptorSize_ = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void SrvManager::ImGui() {

	ImGui::Text("Srv ViewUseIndex: %d / %d", useIndex_, kMaxSRVCount_);
}

/*////////////////////////////////////////////////////////////////////////////////
*									Getter
////////////////////////////////////////////////////////////////////////////////*/
D3D12_CPU_DESCRIPTOR_HANDLE SrvManager::GetCPUHandle(uint32_t index) {

	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap_.Get()->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize_ * index);

	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE SrvManager::GetGPUHandle(uint32_t index) {

	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap_.Get()->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize_ * index);

	return handleGPU;
}

ID3D12DescriptorHeap* SrvManager::GetDescriptorHeap() const { return descriptorHeap_.Get(); }