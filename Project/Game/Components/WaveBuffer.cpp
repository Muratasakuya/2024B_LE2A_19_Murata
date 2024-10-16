#include "WaveBuffer.h"

#include "Engine/Base/NewMoon.h"

void WaveBuffer::Init() {

	DXConstBuffer::Init(NewMoon::GetDXDevice());
}

void WaveBuffer::Update() {

	DXConstBuffer::TransferData(properties);
}

UINT WaveBuffer::GetRootParameterIndex() const {

	return 5;
}
