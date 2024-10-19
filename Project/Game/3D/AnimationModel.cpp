#include "AnimationModel.h"

#include "Engine/Base/NewMoon.h"
#include "Engine/Base/NewMoonGame.h"
#include "Engine/Managers/DXConstBufferManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*						AnimationModel classMethods
////////////////////////////////////////////////////////////////////////////////*/

void AnimationModel::Init(const std::string& modelName, const std::string& animationName) {

	animationName_ = animationName;

	BaseModel::Init(modelName);

	inputVertices_.Init(inputAssembler_.verticesNum.front(), inputAssembler_.vertices.front().GetResource());
	outputVertices_.Init(inputAssembler_.verticesNum.front());

	if (modelData_.meshes.front().material.textureName) {
		pipelineType_ = PipelineType::pObject3D;
	} else {
		pipelineType_ = PipelineType::Object3DUnTex;
	}
}

void AnimationModel::Draw(AnimationTransform transform, MaterialObject3D material, BlendMode blendMode) {

	auto commandList = NewMoon::GetCommandList();
	auto cameraBuffer = NewMoonGame::GameCamera()->GetCamera3D()->GetCameraBuffer();
	auto lightBuffer = NewMoonGame::GetGameLight()->GetLightBuffer();

	SetComputeCommands(animationName_);

	// D3D12_RESOURCE_STATE_UNORDERED_ACCESS -> D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER
	NewMoon::TransitionBarrier(
		outputVertices_.GetResource(),
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

	NewMoon::SetGraphicsPipeline(commandList, pipelineType_, blendMode);
	inputAssembler_.SetBuffer(commandList);
	material.SetCommand(commandList, material.GetRootParameterIndex());
	transform.SetCommand();
	lightBuffer.SetCommand(commandList, lightBuffer.GetRootParameterIndex(pipelineType_));
	cameraBuffer.SetCommand(commandList, cameraBuffer.GetRootParameterIndex(pipelineType_));

	if (modelData_.meshes.front().material.textureName) {
		NewMoon::SetGraphicsRootDescriptorTable(commandList, 2, modelData_.meshes.front().material.textureName.value());
	}
	inputAssembler_.DrawCall(commandList);

	// D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER -> D3D12_RESOURCE_STATE_UNORDERED_ACCESS
	NewMoon::TransitionBarrier(
		outputVertices_.GetResource(),
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
}

void AnimationModel::SetComputeCommands(const std::string& animationName) {

	auto commandList = NewMoon::GetCommandList();

	NewMoon::SetComputePipeline(commandList, ComputePipelineType::SkinningCS);
	commandList->SetComputeRootDescriptorTable(0,
		NewMoonGame::GetModelMangager()->GetSkinClusterData(animationName).paletteSrvHandle.second);
	commandList->SetComputeRootDescriptorTable(1, inputVertices_.GetGpuHandle());
	commandList->SetComputeRootDescriptorTable(2,
		NewMoonGame::GetModelMangager()->GetSkinClusterData(animationName).influenceSrvHandle.second);
	commandList->SetComputeRootDescriptorTable(3, outputVertices_.GetGpuHandle());
	commandList->SetComputeRootConstantBufferView(4, skinningInfoDatas_.GetResource()->GetGPUVirtualAddress());
	// Compute起動
	commandList->Dispatch(static_cast<UINT>(modelData_.meshes.front().vertices.size() + 1023) / 1024, 1, 1);
}