#include "Model.h"

#include "Engine/Base/NewMoon.h"
#include "Engine/Base/NewMoonGame.h"

/*////////////////////////////////////////////////////////////////////////////////
*							Model classMethods
////////////////////////////////////////////////////////////////////////////////*/

void Model::Draw(WorldTransform transform, std::vector<MaterialObject3D>& materials, BlendMode blendMode) {

	auto commandList = NewMoon::GetCommandList();
	auto cameraBuffer = NewMoonGame::GameCamera()->GetCamera3D()->GetCameraBuffer();
	auto lightBuffer = NewMoonGame::GetGameLight()->GetLightBuffer();

	for (uint32_t meshIndex = 0; meshIndex < meshNum_; ++meshIndex) {

		if (modelData_.meshes[meshIndex].material.textureName) {
			pipelineType_ = PipelineType::pObject3D;
		} else {
			pipelineType_ = PipelineType::Object3DUnTex;
		}

		NewMoon::SetGraphicsPipeline(commandList, pipelineType_, blendMode);
		inputAssembler_.SetBuffer(commandList, meshIndex);
		materials[meshIndex].SetCommand(commandList, materials[meshIndex].GetRootParameterIndex());
		transform.SetCommand();
		lightBuffer.SetCommand(commandList, lightBuffer.GetRootParameterIndex(pipelineType_));
		cameraBuffer.SetCommand(commandList, cameraBuffer.GetRootParameterIndex(pipelineType_));
		if (modelData_.meshes[meshIndex].material.textureName) {
			NewMoon::SetGraphicsRootDescriptorTable(commandList, 2, modelData_.meshes[meshIndex].material.textureName.value());
		}
		inputAssembler_.DrawCall(commandList, meshIndex);
	}
}

void Model::SetTexture(const std::string& textureName) {

	for (uint32_t meshIndex = 0; meshIndex < meshNum_; ++meshIndex) {

		modelData_.meshes[meshIndex].material.textureName = textureName;
	}
}

size_t Model::GetMeshNum() const { return meshNum_; }
