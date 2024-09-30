#pragma once

// DirectX
#include <d3d12.h>
#include "ComPtr.h"
#include "PipelineStateStructure.h"

// Lib
#include "Logger.h"

// c++
#include <array>

///===============================================================================
/// クラス前方宣言
class DXCommon;
///===============================================================================

/*////////////////////////////////////////////////////////////////////////////////
*							DXRootSignature Class
////////////////////////////////////////////////////////////////////////////////*/
class DXRootSignature {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	DXRootSignature() = default;
	~DXRootSignature() = default;

	void CreateGraphicsRootSignature(DXCommon* dxCommon, PipelineType pipelineType);
	void CreateComputeRootSignature(DXCommon* dxCommon, ComputePipelineType csPipelineType);

	// Getter
	ID3D12RootSignature* GetGraphicsRootSignature(PipelineType pipelineType) const;
	ID3D12RootSignature* GetComputeRootSignature(ComputePipelineType csPipelineType) const;

private:
	//===================================================================*/
	//							private Variables
	//===================================================================*/

	std::array<ComPtr<ID3D12RootSignature>, pipelineTypeNum> graphicsRootSignature_;
	std::array<ComPtr<ID3D12RootSignature>, computePipelineTypeNum> computeRootSignature_;
	ComPtr<ID3DBlob> signatureBlob_ = nullptr;
	ComPtr<ID3DBlob> errorBlob_ = nullptr;

};