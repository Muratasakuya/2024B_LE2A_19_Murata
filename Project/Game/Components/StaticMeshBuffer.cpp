#include "StaticMeshBuffer.h"

#include "Engine/Base/NewMoon.h"

/*////////////////////////////////////////////////////////////////////////////////
*							StaticMeshBuffer classMethods
////////////////////////////////////////////////////////////////////////////////*/

void StaticMeshBuffer::Init(UINT instanceCount) {

	DXConstBuffer::CreateStructuredBuffer(NewMoon::GetDXDevice(), instanceCount);
}

void StaticMeshBuffer::Update() {

	DXConstBuffer::TransferVectorData(properties);
}

UINT StaticMeshBuffer::GetRootParameterIndex() const {

	return 1;
}