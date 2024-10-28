#include "ParticleBuffer.h"

#include "Engine/Base/NewMoon.h"

/*////////////////////////////////////////////////////////////////////////////////
*							ParticleBuffer classMethods
////////////////////////////////////////////////////////////////////////////////*/

void ParticleBuffer::Init(UINT instanceCount) {

	DXConstBuffer::CreateStructuredBuffer(NewMoon::GetDXDevice(), instanceCount);
}

void ParticleBuffer::Update() {

	DXConstBuffer::TransferVectorData(properties);
}

UINT ParticleBuffer::GetRootParameterIndex() const {

	return 0;
}