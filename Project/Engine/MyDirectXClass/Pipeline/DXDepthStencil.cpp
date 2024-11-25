#include "DXDepthStencil.h"

/*////////////////////////////////////////////////////////////////////////////////
*								DXDepthStencilの生成
////////////////////////////////////////////////////////////////////////////////*/
void DXDepthStencil::Create(PipelineType pipelineType) {

	std::unordered_set<PipelineType> noDepthPipelineTypes = {
	Object2D, OffscreenCopy, OffscreenGrayscale, OffscreenSepiaTone,
	OffscreenVignette, OffscreenSmoothing, OffscreenGaussianFilter,
	OffscreenLuminanceOutline, OffscreenOutline, OffscreenRadialBlur,
	OffscreenDissolve
	};

	if (noDepthPipelineTypes.find(pipelineType) != noDepthPipelineTypes.end()) {

		// Depth機能無効
		depthStencilDesc_.DepthEnable = false;
		depthStencilDesc_.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc_.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	} else {

		// Depth機能有効
		depthStencilDesc_.DepthEnable = true;
		depthStencilDesc_.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc_.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	}

	if (pipelineType == PipelineType::pParticle) {

		// Depth機能無効
		depthStencilDesc_.DepthEnable = true;
		depthStencilDesc_.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
		depthStencilDesc_.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	}

}

/*////////////////////////////////////////////////////////////////////////////////
*									Getter
////////////////////////////////////////////////////////////////////////////////*/
D3D12_DEPTH_STENCIL_DESC DXDepthStencil::GetDesc() { return depthStencilDesc_; }