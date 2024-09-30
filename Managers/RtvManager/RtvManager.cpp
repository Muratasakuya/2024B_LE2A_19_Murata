#include "RtvManager.h"

#include "NewMoon.h"

// SRVの最大数
const uint32_t RtvManager::kMaxRtvCount_ = 32;

// DescriptorHeapを作成する関数
ComPtr<ID3D12DescriptorHeap> RtvManager::MakeDescriptorHeap(ID3D12Device* device) {

	HRESULT hr;

	ComPtr<ID3D12DescriptorHeap> descriptorHeap = nullptr;

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heapDesc.NumDescriptors = kMaxRtvCount_;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	hr = device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&descriptorHeap));
	assert(SUCCEEDED(hr));

	return descriptorHeap;
}

/*////////////////////////////////////////////////////////////////////////////////
*							使用インデックスの計算
////////////////////////////////////////////////////////////////////////////////*/
uint32_t RtvManager::Allocate() {

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
bool RtvManager::CanAllocate() { return useIndex_ < kMaxRtvCount_; }

/*////////////////////////////////////////////////////////////////////////////////
*									RTVの作成
////////////////////////////////////////////////////////////////////////////////*/
void RtvManager::Create(uint32_t index, ID3D12Resource* resource) {

	auto device = NewMoon::GetDXDevice();
	D3D12_CPU_DESCRIPTOR_HANDLE handle = GetCPUHandle(index);

	device->CreateRenderTargetView(resource, &desc_, handle);
}

/*////////////////////////////////////////////////////////////////////////////////
*									初期化
////////////////////////////////////////////////////////////////////////////////*/
void RtvManager::Initialize() {

	// SwapChainのBufferCountの分
	useIndex_ = 2;

	auto device = NewMoon::GetDXDevice();

	// Descの設定
	desc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;        // 出力結果をSRGBも変換して書き込む
	desc_.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;   // 2dテクスチャとして書き込む

	// 生成
	descriptorHeap_ = MakeDescriptorHeap(device);
	// デスクリプタ1個分のサイズ取得
	descriptorSize_ = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
}

/*////////////////////////////////////////////////////////////////////////////////
*							RTV使用インデックスリセット
////////////////////////////////////////////////////////////////////////////////*/
void RtvManager::Reset(){

	useIndex_ = 2;
}

/*////////////////////////////////////////////////////////////////////////////////
*									Getter
////////////////////////////////////////////////////////////////////////////////*/
D3D12_CPU_DESCRIPTOR_HANDLE RtvManager::GetCPUHandle(uint32_t index) {

	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap_.Get()->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize_ * index);

	return handleCPU;
}

D3D12_RENDER_TARGET_VIEW_DESC& RtvManager::GetDesc() { return desc_; }

ID3D12DescriptorHeap* RtvManager::GetDescriptorHeap() const { return descriptorHeap_.Get(); }
