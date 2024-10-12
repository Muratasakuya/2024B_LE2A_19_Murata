#include "PipelineManager.h"

#include "Engine/Base/DXCommon.h"

/*////////////////////////////////////////////////////////////////////////////////
*								 デストラクタ
////////////////////////////////////////////////////////////////////////////////*/
PipelineManager::~PipelineManager() {

	rootSignature_.reset();
	inputLayout_.reset();
	rasterizerState_.reset();
	depthStencil_.reset();
	shaderCompiler_.reset();
}

/*////////////////////////////////////////////////////////////////////////////////
*								PipelineStateの作成
////////////////////////////////////////////////////////////////////////////////*/
void PipelineManager::CreatePipelineState(
	DXCommon* dxCommon, IDxcBlob* vs, IDxcBlob* ps, ID3D12RootSignature* rootSigature, D3D12_INPUT_LAYOUT_DESC inputLayout,
	D3D12_RENDER_TARGET_BLEND_DESC blendDesc, D3D12_RASTERIZER_DESC rasterizerDesc, D3D12_DEPTH_STENCIL_DESC depthStencilDesc,
	PipelineType pipelineType, BlendMode blendMode) {

	HRESULT hr;

	if (pipelineType == PipelineType::PrimitiveLine) {

		// Pipeline State Objectの生成
		graphicsPipelineStateDesc_.pRootSignature = rootSigature;
		graphicsPipelineStateDesc_.InputLayout = inputLayout;
		graphicsPipelineStateDesc_.VS = { vs->GetBufferPointer(),vs->GetBufferSize() };
		graphicsPipelineStateDesc_.PS = { ps->GetBufferPointer(),ps->GetBufferSize() };
		graphicsPipelineStateDesc_.BlendState.RenderTarget[0] = blendDesc;
		graphicsPipelineStateDesc_.RasterizerState = rasterizerDesc;
		graphicsPipelineStateDesc_.DepthStencilState = depthStencilDesc;
		graphicsPipelineStateDesc_.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

		// 書き込むRTVの情報
		graphicsPipelineStateDesc_.NumRenderTargets = 1;
		graphicsPipelineStateDesc_.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		// 利用するトポロジ(形状)のタイプ、三角形
		graphicsPipelineStateDesc_.PrimitiveTopologyType =
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
		// どのように画面に色を打ち込むかの設定
		graphicsPipelineStateDesc_.SampleDesc.Count = 1;
		graphicsPipelineStateDesc_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
		// 実際に生成
		pipelineStates_[pipelineType][blendMode] = nullptr;
		hr = dxCommon->GetDevice()->CreateGraphicsPipelineState(
			&graphicsPipelineStateDesc_,
			IID_PPV_ARGS(&pipelineStates_[pipelineType][blendMode]));
		assert(SUCCEEDED(hr));
	} else {

		// Pipeline State Objectの生成
		graphicsPipelineStateDesc_.pRootSignature = rootSigature;
		graphicsPipelineStateDesc_.InputLayout = inputLayout;
		graphicsPipelineStateDesc_.VS = { vs->GetBufferPointer(),vs->GetBufferSize() };
		graphicsPipelineStateDesc_.PS = { ps->GetBufferPointer(),ps->GetBufferSize() };
		graphicsPipelineStateDesc_.BlendState.RenderTarget[0] = blendDesc;
		graphicsPipelineStateDesc_.RasterizerState = rasterizerDesc;
		graphicsPipelineStateDesc_.DepthStencilState = depthStencilDesc;
		graphicsPipelineStateDesc_.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

		// 書き込むRTVの情報
		graphicsPipelineStateDesc_.NumRenderTargets = 1;
		graphicsPipelineStateDesc_.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		// 利用するトポロジ(形状)のタイプ、三角形
		graphicsPipelineStateDesc_.PrimitiveTopologyType =
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		// どのように画面に色を打ち込むかの設定
		graphicsPipelineStateDesc_.SampleDesc.Count = 1;
		graphicsPipelineStateDesc_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
		// 実際に生成
		pipelineStates_[pipelineType][blendMode] = nullptr;
		hr = dxCommon->GetDevice()->CreateGraphicsPipelineState(
			&graphicsPipelineStateDesc_,
			IID_PPV_ARGS(&pipelineStates_[pipelineType][blendMode]));
		assert(SUCCEEDED(hr));
	}
}

/*////////////////////////////////////////////////////////////////////////////////
*						ComputePipelineStateの作成
////////////////////////////////////////////////////////////////////////////////*/
void PipelineManager::CreateComputePipelineState(
	DXCommon* dxCommon, IDxcBlob* cs, ID3D12RootSignature* rootSigature, ComputePipelineType csPipelineType) {

	HRESULT hr;

	computePipelineStateDesc.CS = {
		.pShaderBytecode = cs->GetBufferPointer(),
		.BytecodeLength = cs->GetBufferSize()
	};
	computePipelineStateDesc.pRootSignature = rootSigature;

	// 生成
	hr = dxCommon->GetDevice()->CreateComputePipelineState(
		&computePipelineStateDesc,
		IID_PPV_ARGS(&computePipelineStates_[csPipelineType]));
	assert(SUCCEEDED(hr));
}

/*////////////////////////////////////////////////////////////////////////////////
*								PSOの作成
////////////////////////////////////////////////////////////////////////////////*/
void PipelineManager::CreatePipelineStateObject(DXCommon* dxCommon) {

	rootSignature_ = std::make_unique<DXRootSignature>();
	inputLayout_ = std::make_unique<DXInputLayout>();
	rasterizerState_ = std::make_unique<DXRasterizerState>();
	depthStencil_ = std::make_unique<DXDepthStencil>();
	shaderCompiler_ = std::make_unique<DXShaderCompiler>();

	for (PipelineType pipelineType : pipelineTypes) {

		// ShaderCompile
		shaderCompiler_->Init(dxCommon, pipelineType);

		for (BlendMode blendType : blendModeTypes) {

			// RootSignature
			rootSignature_->CreateGraphicsRootSignature(dxCommon, pipelineType);

			// InputLayout
			inputLayout_->Create(pipelineType);

			// BlendState
			D3D12_RENDER_TARGET_BLEND_DESC blendState = blendState_.Create(blendType);

			// RasterizerState
			rasterizerState_->Create(pipelineType);

			// depthStencil
			depthStencil_->Create(pipelineType);

			// PipelineState
			CreatePipelineState(
				dxCommon, shaderCompiler_->GetVSBlob(pipelineType), shaderCompiler_->GetPSBlob(pipelineType), rootSignature_->GetGraphicsRootSignature(pipelineType),
				inputLayout_->GetDesc(pipelineType), blendState, rasterizerState_->GetDesc(pipelineType), depthStencil_->GetDesc(), pipelineType, blendType);
		}
	}

	for (ComputePipelineType csPipeline : computePipelineTypes) {

		// ShaderCompile
		shaderCompiler_->Init(dxCommon, csPipeline);

		// RootSignature
		rootSignature_->CreateComputeRootSignature(dxCommon, csPipeline);

		// ComputePipelineState
		CreateComputePipelineState(dxCommon, shaderCompiler_->GetCSBlob(csPipeline), rootSignature_->GetComputeRootSignature(csPipeline), csPipeline);
	}
}

/*////////////////////////////////////////////////////////////////////////////////
*								パイプラインのセット
////////////////////////////////////////////////////////////////////////////////*/
void PipelineManager::SetGraphicsPipeline(
	ID3D12GraphicsCommandList* commandList, PipelineType pipelineType, BlendMode blendMode) {

	// RootSignatureの設定
	commandList->SetGraphicsRootSignature(rootSignature_->GetGraphicsRootSignature(pipelineType));
	// PipelineStateの設定
	commandList->SetPipelineState(pipelineStates_[pipelineType][blendMode].Get());
}
void PipelineManager::SetComputePipeline(ID3D12GraphicsCommandList* commandList, ComputePipelineType csPipelineType) {

	// RootSignatureの設定
	commandList->SetComputeRootSignature(rootSignature_->GetComputeRootSignature(csPipelineType));
	// PipelineStateの設定
	commandList->SetPipelineState(computePipelineStates_[csPipelineType].Get());
}