#include "Model.h"

#include "Engine/Base/NewMoon.h"
#include "Engine/Managers/DXConstBufferManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*									Main
////////////////////////////////////////////////////////////////////////////////*/
void Model::Initialize(const std::string& modelName) {

	// 使用するモデル、テクスチャ
	modelData_ = NewMoon::GetModelData(modelName);
	meshNum_ = modelData_.meshes.size();

	// 頂点数、インデックス数のリサイズ
	verticesNum_.resize(meshNum_);
	indicesNum_.resize(meshNum_);
	vertices_.resize(meshNum_);
	indices_.resize(meshNum_);
	inputVertices_.resize(meshNum_);
	outputVertices_.resize(meshNum_);
	skinningInfoDatas_.resize(meshNum_);

	for (uint32_t meshIndex = 0; meshIndex < meshNum_; ++meshIndex) {

		const auto& mesh = modelData_.meshes[meshIndex];
		verticesNum_[meshIndex] = static_cast<UINT>(mesh.vertices.size());
		indicesNum_[meshIndex] = static_cast<UINT>(mesh.indices.size());

		// ConstBuffer初期化
		vertices_[meshIndex].Initialize(verticesNum_[meshIndex]);
		indices_[meshIndex].Initialize(indicesNum_[meshIndex]);
		skinningInfoDatas_[meshIndex].Initialize(verticesNum_[meshIndex]);

		vertices_[meshIndex].data.resize(verticesNum_[meshIndex]);
		std::copy(mesh.vertices.begin(), mesh.vertices.end(), vertices_[meshIndex].data.begin());

		indices_[meshIndex].data.resize(indicesNum_[meshIndex]);
		std::copy(mesh.indices.begin(), mesh.indices.end(), indices_[meshIndex].data.begin());
	}

	for (uint32_t meshIndex = 0; meshIndex < meshNum_; ++meshIndex) {

		// ConstBuffer転送
		vertices_[meshIndex].Update();
		indices_[meshIndex].Update();

		// ConstBuffer初期化
		inputVertices_[meshIndex].Initialize(verticesNum_[meshIndex], vertices_[meshIndex].GetResource());
		outputVertices_[meshIndex].Initialize(verticesNum_[meshIndex]);
	}
}

void Model::Draw(BlendMode blendMode) {

	auto commandList = NewMoon::GetCommandList();
	DXConstBufferManager constBuffer;

	for (uint32_t meshIndex = 0; meshIndex < meshNum_; ++meshIndex) {

		// パイプラインの設定
		if (modelData_.meshes[meshIndex].material.textureName) {
			pipelineType_ = PipelineType::pObject3D;
		} else {
			pipelineType_ = PipelineType::Object3DUnTex;
		}

		NewMoon::SetGraphicsPipeline(commandList, pipelineType_, blendMode);
		commandList->IASetVertexBuffers(0, 1, &vertices_[meshIndex].GetVertexBuffer());
		commandList->IASetIndexBuffer(&indices_[meshIndex].GetIndexBuffer());
		constBuffer.SetCommands(commandList, pipelineType_, worldTransform_, material_[meshIndex], light_, camera_);
		if (modelData_.meshes[meshIndex].material.textureName) {
			NewMoon::SetGraphicsRootDescriptorTable(commandList, 2, modelData_.meshes[meshIndex].material.textureName.value());
		}
		commandList->DrawIndexedInstanced(static_cast<UINT>(indices_[meshIndex].data.size()), 1, 0, 0, 0);
	}
}
void Model::SkinningAnimationDraw(const std::string& animationName, BlendMode blendMode) {

	auto commandList = NewMoon::GetCommandList();
	DXConstBufferManager constBuffer;

	SetComputeCommands(animationName);

	for (uint32_t meshIndex = 0; meshIndex < meshNum_; ++meshIndex) {

		// D3D12_RESOURCE_STATE_UNORDERED_ACCESS -> D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER
		NewMoon::TransitionBarrier(
			outputVertices_[meshIndex].GetResource(),
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
			D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

		NewMoon::SetGraphicsPipeline(commandList, pipelineType_, blendMode);
		commandList->IASetVertexBuffers(0, 1, &outputVertices_[meshIndex].GetVertexBuffer());
		commandList->IASetIndexBuffer(&indices_[meshIndex].GetIndexBuffer());
		constBuffer.SetCommands(commandList, pipelineType_, worldTransform_, material_[meshIndex], light_, camera_);
		if (modelData_.meshes[meshIndex].material.textureName) {
			NewMoon::SetGraphicsRootDescriptorTable(commandList, 2, modelData_.meshes[meshIndex].material.textureName.value());
		}
		commandList->DrawIndexedInstanced(static_cast<UINT>(indices_[meshIndex].data.size()), 1, 0, 0, 0);

		// D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER -> D3D12_RESOURCE_STATE_UNORDERED_ACCESS
		NewMoon::TransitionBarrier(
			outputVertices_[meshIndex].GetResource(),
			D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
	}
}
void Model::SetComputeCommands(const std::string& animationName) {

	auto commandList = NewMoon::GetCommandList();

	for (uint32_t meshIndex = 0; meshIndex < meshNum_; ++meshIndex) {

		NewMoon::SetComputePipeline(commandList, ComputePipelineType::SkinningCS);
		commandList->SetComputeRootDescriptorTable(0, NewMoon::GetSkinClusterData(animationName).paletteSrvHandle.second);
		commandList->SetComputeRootDescriptorTable(1, inputVertices_[meshIndex].GetGpuHandle());
		commandList->SetComputeRootDescriptorTable(2, NewMoon::GetSkinClusterData(animationName).influenceSrvHandle.second);
		commandList->SetComputeRootDescriptorTable(3, outputVertices_[meshIndex].GetGpuHandle());
		commandList->SetComputeRootConstantBufferView(4, skinningInfoDatas_[meshIndex].GetResource()->GetGPUVirtualAddress());
		// Compute起動
		commandList->Dispatch(static_cast<UINT>(modelData_.meshes[meshIndex].vertices.size() + 1023) / 1024, 1, 1);
	}
}

/*////////////////////////////////////////////////////////////////////////////////
*									Getter
////////////////////////////////////////////////////////////////////////////////*/
size_t Model::GetMeshNum() const { return meshNum_; }