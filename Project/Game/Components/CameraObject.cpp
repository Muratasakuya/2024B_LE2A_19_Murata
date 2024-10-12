#include "CameraObject.h"

#include "Engine/Base/NewMoon.h"

void CameraObject::Init() {

	camera.worldPosition.Init();
	DXConstBuffer::Init(NewMoon::GetDXDevice());
}

void CameraObject::Update(const Vector3& worldPos) {

	// ワールド座標取得
	camera.worldPosition = worldPos;

	DXConstBuffer::TransferData(camera);
}

void ViewProjectionBuffer::Init() {

	viewProjection.MakeIdentity4x4();
	DXConstBuffer::Init(NewMoon::GetDXDevice());
}

void ViewProjectionBuffer::Update(Matrix4x4 viewPro) {

	viewProjection = viewPro;
	DXConstBuffer::TransferData(viewProjection);
}