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

	inputVertices_.Init(static_cast<UINT>(inputAssembler_.GetVertexData().data.size()),
		inputAssembler_.GetVertexData().GetResource());
	outputVertices_.Init(static_cast<UINT>(inputAssembler_.GetVertexData().data.size()));

	skinningInfoDates_.Init(static_cast<UINT>(inputAssembler_.GetVertexData().data.size()));

	if (modelData_.meshes.front().material.textureName) {
		pipelineType_ = PipelineType::pObject3D;
	} else {
		pipelineType_ = PipelineType::Object3DUnTex;
	}

}

void AnimationModel::Draw(AnimationTransform transform, MaterialObject3D material, BlendMode blendMode) {

	auto commandList = NewMoon::GetCommandList();

	SetComputeCommands(animationName_);

	// D3D12_RESOURCE_STATE_UNORDERED_ACCESS -> D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER
	NewMoon::TransitionBarrier(
		outputVertices_.GetResource(),
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

	NewMoon::SetGraphicsPipeline(commandList, pipelineType_, blendMode);
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList->IASetVertexBuffers(0, 1, &outputVertices_.GetVertexBuffer());
	commandList->IASetIndexBuffer(&inputAssembler_.GetIndexData().GetIndexBuffer());
	material.SetCommand(commandList);
	transform.SetCommand(commandList);
	NewMoonGame::SetEnvironmentCommand(commandList, pipelineType_);
	if (modelData_.meshes.front().material.textureName) {
		NewMoon::SetGraphicsRootDescriptorTable(commandList, 2, modelData_.meshes.front().material.textureName.value());
	}
	inputAssembler_.DrawCall(commandList, 0);

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
		NewMoonGame::GetModelManager()->GetSkinClusterData(animationName).paletteSrvHandle.second);
	commandList->SetComputeRootDescriptorTable(1, inputVertices_.GetGpuHandle());
	commandList->SetComputeRootDescriptorTable(2,
		NewMoonGame::GetModelManager()->GetSkinClusterData(animationName).influenceSrvHandle.second);
	commandList->SetComputeRootDescriptorTable(3, outputVertices_.GetGpuHandle());
	commandList->SetComputeRootConstantBufferView(4, skinningInfoDates_.GetResource()->GetGPUVirtualAddress());
	// Compute起動
	commandList->Dispatch(static_cast<UINT>(modelData_.meshes.front().vertices.size() + 1023) / 1024, 1, 1);

}

void AnimationModel::SetAnimationName(const std::string& animationName) {
	animationName_ = animationName;
}