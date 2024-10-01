#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Engine/MyDirectXClass/ComPtr.h"
#include "Engine/MyDirectXClass/Pipeline/PipelineStateStructure.h"

// directX
#include <d3d12.h>

/*////////////////////////////////////////////////////////////////////////////////
*								DXBlendState Class
////////////////////////////////////////////////////////////////////////////////*/
class DXBlendState {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	// default
	DXBlendState() = default;
	~DXBlendState() = default;

	D3D12_RENDER_TARGET_BLEND_DESC Create(BlendMode blendMode);

};