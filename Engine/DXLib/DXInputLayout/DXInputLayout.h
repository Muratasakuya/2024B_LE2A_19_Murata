#pragma once

// DirectX
#include <d3d12.h>
#include "ComPtr.h"
#include "PipelineStateStructure.h"

// c++
#include <array>

/*////////////////////////////////////////////////////////////////////////////////
*							DXInputLayout Class
////////////////////////////////////////////////////////////////////////////////*/
class DXInputLayout {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	DXInputLayout() = default;
	~DXInputLayout() = default;

	void Create(PipelineType pipelineType);

	// Getter
	D3D12_INPUT_LAYOUT_DESC GetDesc(PipelineType pipelineType);

private:
	//===================================================================*/
	//							private Variable
	//===================================================================*/

	std::array<D3D12_INPUT_LAYOUT_DESC, pipelineTypeNum> inputLayoutDesc_{};

};