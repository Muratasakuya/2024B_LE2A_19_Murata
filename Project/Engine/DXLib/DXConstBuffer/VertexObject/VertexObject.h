#pragma once

// DXLib
#include "DXConstBuffer.h"

// c++
#include <cstdint>

/*////////////////////////////////////////////////////////////////////////////////
*								VertexObject Class
////////////////////////////////////////////////////////////////////////////////*/
template<typename T>
class VertexObject :
	public DXConstBuffer<T> {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	VertexObject() = default;
	~VertexObject() = default;

	void Initialize(UINT vertexNum);
	void Update();

public:
	//===================================================================*/
	//							public Variable
	//===================================================================*/

	std::vector<T> data;

};

/*////////////////////////////////////////////////////////////////////////////////
*								InputVertexObject Class
////////////////////////////////////////////////////////////////////////////////*/
class InputVertexObject :
	public DXConstBuffer<VertexData3D> {
private:
	//===================================================================*/
	//							private Struct
	//===================================================================*/

	// Input
	struct InputVertex {

		std::vector<VertexData3D> data;
		uint32_t srvIndex;
		std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE> srvHandle;
	};

public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	InputVertexObject() = default;
	~InputVertexObject() = default;

	void Initialize(UINT vertexNum, ID3D12Resource* vertexResource);

	// Getter
	D3D12_GPU_DESCRIPTOR_HANDLE& GetGpuHandle();

public:
	//===================================================================*/
	//							public Variables
	//===================================================================*/

	InputVertex inputVertex;

};

/*////////////////////////////////////////////////////////////////////////////////
*								OutputVertexObject Class
////////////////////////////////////////////////////////////////////////////////*/
class OutputVertexObject :
	public DXConstBuffer<VertexData3D> {
private:
	//===================================================================*/
	//							private Struct
	//===================================================================*/

	// Output
	struct OutputVertex {

		std::vector<VertexData3D> data;
		uint32_t uavIndex;
		std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE> uavHandle;
	};

public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	OutputVertexObject() = default;
	~OutputVertexObject() = default;

	void Initialize(UINT vertexNum);

	// Getter
	D3D12_GPU_DESCRIPTOR_HANDLE& GetGpuHandle();

public:
	//===================================================================*/
	//							public Variables
	//===================================================================*/

	OutputVertex outputVertex;

};

/*////////////////////////////////////////////////////////////////////////////////
*								SkinningInfoData Class
////////////////////////////////////////////////////////////////////////////////*/
class SkinningInfoData :
	public DXConstBuffer<SkinningInformation> {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	SkinningInfoData() = default;
	~SkinningInfoData() = default;

	void Initialize(UINT vertexNum);

public:
	//===================================================================*/
	//							public Variables
	//===================================================================*/

	SkinningInformation info;

};