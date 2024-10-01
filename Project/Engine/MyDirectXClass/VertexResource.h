#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Engine/MyDirectXClass/ComPtr.h"

// directX
#include <d3d12.h>

// c++
#include <cstdint>
#include <memory>
#include <cassert>

/*////////////////////////////////////////////////////////////////////////////////
*								VertexResource Class
////////////////////////////////////////////////////////////////////////////////*/
class VertexResource{
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	// BufferResource生成
	ComPtr<ID3D12Resource> CreateBufferResource(ID3D12Device* device, size_t sizeInBytes);

};