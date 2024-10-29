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

Vector3 BaseTransform::GetWorldPos() const {

	Vector3 worldPos{};
	worldPos.x = matrix.World.m[3][0];
	worldPos.y = matrix.World.m[3][1];
	worldPos.z = matrix.World.m[3][2];

	return worldPos;
}