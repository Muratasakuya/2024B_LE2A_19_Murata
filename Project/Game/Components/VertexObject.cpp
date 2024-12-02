#include "VertexObject.h"

#include "Engine/Base/NewMoon.h"
#include "Engine/Managers/SrvManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*						PrimitiveVertexObject Class
////////////////////////////////////////////////////////////////////////////////*/
void PrimitiveVertexObject::Init(UINT vertexNum) {

	DXConstBuffer::CreateVertexBuffer(NewMoon::GetDXDevice(), vertexNum);
}

void PrimitiveVertexObject::Update() {

	DXConstBuffer::TransferVectorData(pos);
}

template class VertexObject<VertexData3D>;
template class VertexObject<VertexData2D>;

/*////////////////////////////////////////////////////////////////////////////////
*								VertexObject Class
////////////////////////////////////////////////////////////////////////////////*/
template<>
void VertexObject<VertexData3D>::Init(UINT vertexNum) {

	DXConstBuffer<VertexData3D>::CreateVertexBuffer(NewMoon::GetDXDevice(), vertexNum);
}
template<>
void VertexObject<VertexData3D>::Update() {

	TransferVectorData(data);
}

template<>
void VertexObject<VertexData2D>::Init(UINT vertexNum) {

	DXConstBuffer<VertexData2D>::CreateVertexBuffer(NewMoon::GetDXDevice(), vertexNum);
}
template<>
void VertexObject<VertexData2D>::Update() {

	TransferVectorData(data);
}

/*////////////////////////////////////////////////////////////////////////////////
*								InputVertexObject Class
////////////////////////////////////////////////////////////////////////////////*/
void InputVertexObject::Init(UINT vertexNum, ID3D12Resource* vertexResource) {

	// SRV確保
	inputVertex.srvIndex = NewMoon::GetSrvManagerPtr()->Allocate("inputResource");
	inputVertex.srvHandle.first = NewMoon::GetSrvManagerPtr()->GetCPUHandle(inputVertex.srvIndex);
	inputVertex.srvHandle.second = NewMoon::GetSrvManagerPtr()->GetGPUHandle(inputVertex.srvIndex);
	// SRV生成
	NewMoon::GetSrvManagerPtr()->CreateSRVForStructureBuffer(
		inputVertex.srvIndex, vertexResource, vertexNum, static_cast<UINT>(sizeof(VertexData3D)));
}
D3D12_GPU_DESCRIPTOR_HANDLE& InputVertexObject::GetGpuHandle() { return inputVertex.srvHandle.second; }

/*////////////////////////////////////////////////////////////////////////////////
*								OutputVertexObject Class
////////////////////////////////////////////////////////////////////////////////*/
void OutputVertexObject::Init(UINT vertexNum) {

	DXConstBuffer<VertexData3D>::CreateUavVertexBuffer(NewMoon::GetDXDevice(), vertexNum);

	// UAV確保
	outputVertex.uavIndex = NewMoon::GetSrvManagerPtr()->Allocate("outputResource");
	outputVertex.uavHandle.first = NewMoon::GetSrvManagerPtr()->GetCPUHandle(outputVertex.uavIndex);
	outputVertex.uavHandle.second = NewMoon::GetSrvManagerPtr()->GetGPUHandle(outputVertex.uavIndex);
	// UAV生成
	NewMoon::GetSrvManagerPtr()->CreateUAVForStructureBuffer(
		outputVertex.uavIndex, this->GetResource(), vertexNum, static_cast<UINT>(sizeof(VertexData3D)));
}
D3D12_GPU_DESCRIPTOR_HANDLE& OutputVertexObject::GetGpuHandle() { return outputVertex.uavHandle.second; }

/*////////////////////////////////////////////////////////////////////////////////
*								SkinningInfoData Class
////////////////////////////////////////////////////////////////////////////////*/
void SkinningInfoData::Init(UINT vertexNum) {

	info.numVertices = vertexNum;

	DXConstBuffer::Init(NewMoon::GetDXDevice());
	TransferData(info);
}