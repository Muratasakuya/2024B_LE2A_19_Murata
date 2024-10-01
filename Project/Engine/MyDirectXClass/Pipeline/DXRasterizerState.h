#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Engine/MyDirectXClass/ComPtr.h"
#include "Engine/MyDirectXClass/Pipeline/PipelineStateStructure.h"

// directX
#include <d3d12.h>

// c++
#include <array>
#include <unordered_set>

/*////////////////////////////////////////////////////////////////////////////////
*								DXBlendState Class
////////////////////////////////////////////////////////////////////////////////*/
class DXRasterizerState {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	DXRasterizerState() = default;
	~DXRasterizerState() = default;

	void Create(PipelineType pipelineType);

	// Getter
	D3D12_RASTERIZER_DESC GetDesc(PipelineType pipelineType);

private:
	//===================================================================*/
	//							private Variable
	//===================================================================*/

	std::array<D3D12_RASTERIZER_DESC, pipelineTypeNum> rasterizerDesc_{};

};