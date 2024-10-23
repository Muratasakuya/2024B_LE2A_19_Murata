#include "TextureManager.h"

#include "Engine/Base/NewMoon.h"
#include "Engine/Base/DXCommon.h"
#include "Engine/Managers/SrvManager.h"
#include "Engine/MyDirectXClass/VertexResource.h"

const DirectX::TexMetadata& TextureManager::GetMetaData(const std::string textureName) {

	auto it = textures_.find(textureName);
	if (it == textures_.end()) {

		throw std::runtime_error("Texture not found: " + textureName);
	}

	TextureData& textureData = it->second;

	return textureData.metadata;
}

D3D12_GPU_DESCRIPTOR_HANDLE& TextureManager::GetTextureGpuHandle(const std::string textureName) {

	auto it = textures_.find(textureName);
	if (it == textures_.end()) {

		throw std::runtime_error("Texture not found: " + textureName);
	}

	TextureData& textureData = it->second;

	return textureData.gpuHandle;
}

const std::vector<std::string>& TextureManager::GetAllTextureNames() const {

	return textureNames_;
}

// DescriptorHandleCPUの生成
D3D12_CPU_DESCRIPTOR_HANDLE TextureManager::GetCPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index) {

	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize * index);

	return handleCPU;
}

// DescriptorHandleGPUの生成
D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetGPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index) {

	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize * index);

	return handleGPU;
}

/*////////////////////////////////////////////////////////////////////////////////
*							   Textureデータの読み込み
////////////////////////////////////////////////////////////////////////////////*/
DirectX::ScratchImage TextureManager::Load(const std::string& filePath) {

	// テクスチャファイルを呼んでプログラムを扱えるようにする
	DirectX::ScratchImage image{};
	std::wstring filePathW = ConvertString(filePath);
	HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr));

	// ミップマップの作成 → 元画像よりも小さなテクスチャ群
	DirectX::ScratchImage mipImages{};
	hr = DirectX::GenerateMipMaps(
		image.GetImages(), image.GetImageCount(), image.GetMetadata(),
		DirectX::TEX_FILTER_SRGB, 0, mipImages);
	assert(SUCCEEDED(hr));

	// ミップマップ付きのデータを返す
	return mipImages;
}

/*////////////////////////////////////////////////////////////////////////////////
*						    TextureResourceを作成する関数
////////////////////////////////////////////////////////////////////////////////*/
ComPtr<ID3D12Resource> TextureManager::CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata) {

	HRESULT hr;

	// metadataを元にResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = UINT(metadata.width);                             // Textureの幅
	resourceDesc.Height = UINT(metadata.height);                           // Textureの高さ
	resourceDesc.MipLevels = UINT16(metadata.mipLevels);                   // mipmapの数
	resourceDesc.DepthOrArraySize = UINT16(metadata.arraySize);            // 奥行き　or 配列textureの配列数
	resourceDesc.Format = metadata.format;                                 // TextureのFormat
	resourceDesc.SampleDesc.Count = 1;                                     // サンプリングカウント。1固定
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension); // Textureの次元数、普段は2次元

	// 利用するHeapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

	// Resourceの作成
	ComPtr<ID3D12Resource> resource = nullptr;
	hr =
		device->CreateCommittedResource(
			&heapProperties,                   // Heapの設定
			D3D12_HEAP_FLAG_NONE,              // Heapの特殊な設定
			&resourceDesc,                     // Resourceの設定
			D3D12_RESOURCE_STATE_COPY_DEST,    // 初回のResourceState、Textureは基本読むだけ
			nullptr,                           // Clear最適値、使わない。
			IID_PPV_ARGS(&resource)            // 作成するResourceポインタへのポインタ
		);
	assert(SUCCEEDED(hr));

	return resource;
}

/*////////////////////////////////////////////////////////////////////////////////
*						  TextureResourceにデータを転送する関数
////////////////////////////////////////////////////////////////////////////////*/
[[nodiscard]]
ComPtr<ID3D12Resource> TextureManager::UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages) {

	std::vector<D3D12_SUBRESOURCE_DATA> subresources;
	VertexResource resource{};
	auto commandList = NewMoon::GetCommandList();

	DirectX::PrepareUpload(NewMoon::GetDXDevice(), mipImages.GetImages(), mipImages.GetImageCount(), mipImages.GetMetadata(), subresources);
	uint64_t intermediateSize = GetRequiredIntermediateSize(texture, 0, static_cast<UINT>(subresources.size()));
	ComPtr<ID3D12Resource> intermediateResource = resource.CreateBufferResource(NewMoon::GetDXDevice(), intermediateSize);
	UpdateSubresources(commandList, texture, intermediateResource.Get(), 0, 0, static_cast<UINT>(subresources.size()), subresources.data());

	// Textureへの転送後は利用できるようにする
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = texture;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;

	commandList->ResourceBarrier(1, &barrier);

	return intermediateResource;
}

/*////////////////////////////////////////////////////////////////////////////////
*							  動的なテクスチャ読み込み関数
////////////////////////////////////////////////////////////////////////////////*/
void TextureManager::LoadTexture(const std::string& textureName) {

	// defaultFilePath
	std::string baseDirectory = "./Resources/Images/";
	std::filesystem::path filePath;

	bool found = false;
	for (const auto& entry : std::filesystem::recursive_directory_iterator(baseDirectory)) {
		if (entry.is_regular_file() && entry.path().stem().string() == textureName) {

			filePath = entry.path();
			found = true;

			break;
		}
	}

	// テクスチャなし
	assert(found && "Error: Texture not found in directory or its subdirectories.");

	// ファイルパスからファイル名を取得
	std::string identifier = filePath.stem().string();
	textureNames_.push_back(identifier);

	// テクスチャデータを追加して書き込む
	TextureData& texture = textures_[identifier];
	// テクスチャを読み込む
	DirectX::ScratchImage mipImages = Load(filePath.string());
	texture.metadata = mipImages.GetMetadata();
	texture.resource = CreateTextureResource(dxCommon_->GetDevice(), texture.metadata);
	ComPtr<ID3D12Resource> intermediateResource = UploadTextureData(texture.resource.Get(), mipImages);

	// GPU実行の完了を待つ
	dxCommon_->WaitForGPU();

	intermediateResource.Reset();

	// SRV確保
	texture.srvIndex = srvManager_->Allocate();
	texture.cpuHandle = srvManager_->GetCPUHandle(texture.srvIndex);
	texture.gpuHandle = srvManager_->GetGPUHandle(texture.srvIndex);
	srvManager_->CreateSRVForTexture2D(
		texture.srvIndex, texture.resource.Get(), texture.metadata.format, UINT(texture.metadata.mipLevels));

	// 読み込み済みテクスチャの検索
	if (textures_.contains(identifier)) {

		assert(srvManager_->CanAllocate());
	}
}

/*////////////////////////////////////////////////////////////////////////////////
*							テクスチャassert あるかチェック
////////////////////////////////////////////////////////////////////////////////*/
void TextureManager::CheckAvailability(const std::string name) {

	// nameがtextures_の中にあるかチェック
	auto it = textures_.find(name);

	// ない場合はassert
	if (it == textures_.end()) {

		assert(false && "Texture not found");
	}
}

/*////////////////////////////////////////////////////////////////////////////////
*									SRVのセット
////////////////////////////////////////////////////////////////////////////////*/
void TextureManager::SetGraphicsRootDescriptorTable(
	ID3D12GraphicsCommandList* commandList, UINT rootParamaterIndex, std::string identifier) {

	commandList->SetGraphicsRootDescriptorTable(rootParamaterIndex, textures_[identifier].gpuHandle);
}

/*////////////////////////////////////////////////////////////////////////////////
*								 インスタンスの代入
////////////////////////////////////////////////////////////////////////////////*/
void TextureManager::Init() {

	dxCommon_ = NewMoon::GetDXCommonPtr();
	srvManager_ = NewMoon::GetSrvManagerPtr();
}