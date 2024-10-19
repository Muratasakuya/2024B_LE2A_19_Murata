#include "BaseTransform.h"

#include "Engine/Base/NewMoon.h"

/*////////////////////////////////////////////////////////////////////////////////
*						BaseTransform classMethods
////////////////////////////////////////////////////////////////////////////////*/

void BaseTransform::Init() {

	scale.SetInit(1.0f);
	rotation.Init();
	translation.Init();

	DXConstBuffer::Init(NewMoon::GetDXDevice());
}

void BaseTransform::SetCommand() {

	auto commandList = NewMoon::GetCommandList();

	DXConstBuffer::SetCommand(commandList, rootParameterIndex_);
}