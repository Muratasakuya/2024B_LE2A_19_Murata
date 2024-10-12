#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Engine/MyDirectXClass/ComPtr.h"
#include "Engine/MyDirectXClass/Logger.h"

// directX
#include <d3d12.h>
#include "Externals/DirectXTex/DirectXTex.h"
#include "Externals/DirectXTex/d3dx12.h"

// c++
#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <iostream> 

///===============================================================================
/// クラス前方宣言
class DXCommon;
class SrvManager;
///===============================================================================

/*////////////////////////////////////////////////////////////////////////////////
*							TextureManager Class
////////////////////////////////////////////////////////////////////////////////*/
class TextureManager {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	TextureManager() = default;
	~TextureManager() = default;

	void LoadTexture(const std::string& textureName);
	void CheckAvailability(const std::string name);

	void SetGraphicsRootDescriptorTable(
		ID3D12GraphicsCommandList* commandList, UINT rootParamaterIndex, std::string identifier);

	void Init();

	// Getter
	const DirectX::TexMetadata& GetMetaData(const std::string textureName);

private:
	//===================================================================*/
	//							private Variables
	//===================================================================*/

	DXCommon* dxCommon_ = nullptr;
	SrvManager* srvManager_ = nullptr;

	struct TextureData {

		ComPtr<ID3D12Resource> resource;
		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
		D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle;
		DirectX::TexMetadata metadata;
		uint32_t srvIndex;
	};

	std::unordered_map<std::string, TextureData> textures_;

private:
	//===================================================================*/
	//							private Functions
	//===================================================================*/

	DirectX::ScratchImage Load(const std::string& filePath);
	ComPtr<ID3D12Resource> CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata);
	ComPtr<ID3D12Resource> UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index);

};