#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Engine/MyDirectXClass/ComPtr.h"
#include "Engine/MyDirectXClass/Pipeline/PipelineStateStructure.h"

// directX
#include <d3d12.h>

// c++
#include <unordered_set>

/*////////////////////////////////////////////////////////////////////////////////
*								DXDepthStencil Class
////////////////////////////////////////////////////////////////////////////////*/
class DXDepthStencil{
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	DXDepthStencil() = default;
	~DXDepthStencil() = default;

	void Create(PipelineType pipelineType);

	// Getter
	D3D12_DEPTH_STENCIL_DESC GetDesc();

private:
	//===================================================================*/
	//							private Variable
	//===================================================================*/

	D3D12_DEPTH_STENCIL_DESC depthStencilDesc_{};

};

