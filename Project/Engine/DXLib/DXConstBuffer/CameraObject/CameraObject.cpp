#include "CameraObject.h"

#include "NewMoon.h"

void CameraObject::Initialize() {

	camera.worldPosition.Initialize();

	// 定数バッファ生成
	DXConstBuffer::Initialize(NewMoon::GetDXDevice());
}

void CameraObject::Update(const Vector3& worldPos) {

	// ワールド座標取得
	camera.worldPosition = worldPos;

	// 定数バッファにデータを転送
	TransferData(camera);
}