#include "DXRasterizerState.h"

/*////////////////////////////////////////////////////////////////////////////////
*							DXRasterizerStateの生成
////////////////////////////////////////////////////////////////////////////////*/
void DXRasterizerState::Create(PipelineType pipelineType) {

	std::unordered_set<PipelineType> noCullPipelineTypes = {
	Object2D, OffscreenCopy, OffscreenGrayscale, OffscreenSepiaTone,
	OffscreenVignette, OffscreenSmoothing, OffscreenGaussianFilter,
	OffscreenLuminanceOutline, OffscreenOutline, OffscreenRadialBlur,
	OffscreenDissolve
	};

	if (noCullPipelineTypes.find(pipelineType) != noCullPipelineTypes.end()) {

		// 裏面(時計周り)を表示、背面カリングしない
		rasterizerDesc_[pipelineType].CullMode = D3D12_CULL_MODE_NONE;
	} else {

		// 裏面(時計周り)を表示しない、背面カリング
		rasterizerDesc_[pipelineType].CullMode = D3D12_CULL_MODE_BACK;
	}

	// 三角形の中を塗りつぶす
	rasterizerDesc_[pipelineType].FillMode = D3D12_FILL_MODE_SOLID;
}

/*////////////////////////////////////////////////////////////////////////////////
*									Getter
////////////////////////////////////////////////////////////////////////////////*/
D3D12_RASTERIZER_DESC DXRasterizerState::GetDesc(PipelineType pipelineType) { return rasterizerDesc_[pipelineType]; }