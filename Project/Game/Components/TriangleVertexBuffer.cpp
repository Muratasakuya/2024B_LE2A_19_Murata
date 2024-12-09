#include "TriangleVertexBuffer.h"

#include "Engine/Base/NewMoon.h"

/*////////////////////////////////////////////////////////////////////////////////
*							TriangleVertexBuffer classMethods
////////////////////////////////////////////////////////////////////////////////*/

void TriangleVertexBuffer::Init(UINT instanceCount) {

	DXConstBuffer::CreateStructuredBuffer(NewMoon::GetDXDevice(), instanceCount);
}

void TriangleVertexBuffer::Update() {

	DXConstBuffer::TransferVectorData(properties);
}

UINT TriangleVertexBuffer::GetRootParameterIndex() const {

	return 2;
}