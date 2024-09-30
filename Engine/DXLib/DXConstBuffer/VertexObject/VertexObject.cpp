#include "VertexObject.h"

#include "NewMoon.h"
#include "SrvManager.h"

template class VertexObject<VertexData3D>;
template class VertexObject<VertexData2D>;

/*////////////////////////////////////////////////////////////////////////////////
*								VertexObject Class
////////////////////////////////////////////////////////////////////////////////*/
template<>
void VertexObject<VertexData3D>::Initialize(UINT vertexNum) {

	// 定数バッファ生成
	DXConstBuffer<VertexData3D>::CreateVertexBuffer(NewMoon::GetDXDevice(), vertexNum);
}
template<>
void VertexObject<VertexData3D>::Update() {

	// 定数バッファにデータを転送
	TransferVertexData(data);
}

template<>
void VertexObject<VertexData2D>::Initialize(UINT vertexNum) {

	// 定数バッファ生成
	DXConstBuffer<VertexData2D>::CreateVertexBuffer(NewMoon::GetDXDevice(), vertexNum);
}
template<>
void VertexObject<VertexData2D>::Update() {

	// 定数バッファにデータを転送
	TransferVertexData(data);
}

/*////////////////////////////////////////////////////////////////////////////////
*								InputVertexObject Class
////////////////////////////////////////////////////////////////////////////////*/
void InputVertexObject::Initialize(UINT vertexNum, ID3D12Resource* vertexResource) {

	// SRV確保
	inputVertex.srvIndex = NewMoon::GetSrvManagerPtr()->Allocate();
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
void OutputVertexObject::Initialize(UINT vertexNum) {

	// 定数バッファ生成
	DXConstBuffer<VertexData3D>::CreateUavVertexBuffer(NewMoon::GetDXDevice(), vertexNum);

	// UAV確保
	outputVertex.uavIndex = NewMoon::GetSrvManagerPtr()->Allocate();
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
void SkinningInfoData::Initialize(UINT vertexNum) {

	info.numVertices = vertexNum;

	// 定数バッファ生成
	DXConstBuffer::Initialize(NewMoon::GetDXDevice());
	// 定数バッファ転送
	TransferData(info);
}