#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Engine/MyDirectXClass/ComPtr.h"
#include "Lib/Math/Vector4.h"

// directX
#include <d3d12.h>

// c++
#include <cstdint>
#include <cassert>

/*////////////////////////////////////////////////////////////////////////////////
*							DXOffscreenRender Class
////////////////////////////////////////////////////////////////////////////////*/
class DXOffscreenRender {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	DXOffscreenRender() = default;
	~DXOffscreenRender() = default;

	ComPtr<ID3D12Resource> CreateRenderTextureResource(ID3D12Device* device, uint32_t width, uint32_t height,
		DXGI_FORMAT format, const Vector4& clearColor);

private:
	//===================================================================*/
	//							public Variables
	//===================================================================*/



};

