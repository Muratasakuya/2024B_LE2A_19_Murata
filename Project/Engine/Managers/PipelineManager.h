#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Engine/MyDirectXClass/Pipeline/DXRootSignature.h"
#include "Engine/MyDirectXClass/Pipeline/DXInputLayout.h"
#include "Engine/MyDirectXClass/Pipeline/DXBlendState.h"
#include "Engine/MyDirectXClass/Pipeline/DXRasterizerState.h"
#include "Engine/MyDirectXClass/Pipeline/DXDepthStencil.h"
#include "Engine/MyDirectXClass/Pipeline/DXShaderCompiler.h"

// directX
#include <d3d12.h>
#include <dxgidebug.h>
#include <dxgi1_6.h>
#include <dxcapi.h>

// c++
#include <memory>
#include <array>
#include <cassert>

///===============================================================================
/// クラス前方宣言
class DXCommon;
///===============================================================================

/*////////////////////////////////////////////////////////////////////////////////
*							PipelineManager Class
////////////////////////////////////////////////////////////////////////////////*/
class PipelineManager {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	PipelineManager() {};
	~PipelineManager();

	void CreatePipelineStateObject(DXCommon* dxCommon);

	void SetGraphicsPipeline(ID3D12GraphicsCommandList* commandList, PipelineType pipelineType, BlendMode blendMode);
	void SetComputePipeline(ID3D12GraphicsCommandList* commandList, ComputePipelineType csPipelineType);

private:
	//===================================================================*/
	//							private Variables
	//===================================================================*/

	DXBlendState blendState_;

	std::unique_ptr<DXRootSignature> rootSignature_;
	std::unique_ptr<DXInputLayout> inputLayout_;
	std::unique_ptr<DXRasterizerState> rasterizerState_;
	std::unique_ptr<DXDepthStencil> depthStencil_;
	std::unique_ptr<DXShaderCompiler> shaderCompiler_;

	std::array<std::array<ComPtr<ID3D12PipelineState>, blendModeNum>, pipelineTypeNum> pipelineStates_;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc_{};

	std::array<ComPtr<ID3D12PipelineState>, computePipelineTypeNum> computePipelineStates_;
	D3D12_COMPUTE_PIPELINE_STATE_DESC computePipelineStateDesc{};

private:
	//===================================================================*/
	//							private Function
	//===================================================================*/

	void CreatePipelineState(
		DXCommon* dxCommon, IDxcBlob* vs, IDxcBlob* ps, ID3D12RootSignature* rootSigature, D3D12_INPUT_LAYOUT_DESC inputLayout,
		D3D12_RENDER_TARGET_BLEND_DESC blendDesc, D3D12_RASTERIZER_DESC rasterizerDesc, D3D12_DEPTH_STENCIL_DESC depthStencilDesc,
		PipelineType pipelineType, BlendMode blendMode);

	void CreateComputePipelineState(DXCommon* dxCommon, IDxcBlob* cs, ID3D12RootSignature* rootSigature, ComputePipelineType csPipelineType);
};