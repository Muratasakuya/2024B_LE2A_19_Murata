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

UINT CameraObject::GetRootParameterIndex(const PipelineType& pipelineType) const {

	if (pipelineType == PipelineType::pObject3D ||
		pipelineType == PipelineType::SkinningObject3D ||
		pipelineType == PipelineType::RailNoneBackCulling) {

		return 4;
	}
	if (pipelineType == PipelineType::Object3DUnTex) {

		return 3;
	}

	return UINT_MAX;
}

void ViewProjectionBuffer::Init() {

	viewProjection.MakeIdentity4x4();
	DXConstBuffer::Init(NewMoon::GetDXDevice());
}

void ViewProjectionBuffer::Update(Matrix4x4 viewPro) {

	viewProjection = viewPro;
	DXConstBuffer::TransferData(viewProjection);
}

UINT ViewProjectionBuffer::GetRootParameterIndex() const {

	return 1;
}
