#include "Model.h"

#include "Engine/Base/NewMoon.h"
#include "Engine/Base/NewMoonGame.h"
#include "Engine/Managers/DXConstBufferManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*							Model classMethods
////////////////////////////////////////////////////////////////////////////////*/

void Model::Init(const std::string& modelName) {

	// 使用するモデル、テクスチャ
	modelData_ = NewMoonGame::GetModelMangager()->GetModelData(modelName);
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
		vertices_[meshIndex].Init(verticesNum_[meshIndex]);
		indices_[meshIndex].Init(indicesNum_[meshIndex]);
		skinningInfoDatas_[meshIndex].Init(verticesNum_[meshIndex]);

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
		inputVertices_[meshIndex].Init(verticesNum_[meshIndex], vertices_[meshIndex].GetResource());
		outputVertices_[meshIndex].Init(verticesNum_[meshIndex]);
	}
}

void Model::Draw(WorldTransform transform, std::vector<MaterialObject3D>& materials, BlendMode blendMode) {

	auto commandList = NewMoon::GetCommandList();
	auto cameraBuffer = NewMoonGame::GetGameCamera()->GetCameraBuffer();
	auto lightBuffer = NewMoonGame::GetGameLight()->GetLightBuffer();

	for (uint32_t meshIndex = 0; meshIndex < meshNum_; ++meshIndex) {

		// パイプラインの設定
		if (modelData_.meshes[meshIndex].material.textureName) {
			pipelineType_ = PipelineType::pObject3D;
		} else {
			pipelineType_ = PipelineType::Object3DUnTex;
		}

		NewMoon::SetGraphicsPipeline(commandList, pipelineType_, blendMode);
		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		commandList->IASetVertexBuffers(0, 1, &vertices_[meshIndex].GetVertexBuffer());
		commandList->IASetIndexBuffer(&indices_[meshIndex].GetIndexBuffer());

		materials[meshIndex].SetCommand(commandList, materials[meshIndex].GetRootParameterIndex());
		transform.SetCommand(commandList, transform.GetRootParameterIndex());
		lightBuffer.SetCommand(commandList, lightBuffer.GetRootParameterIndex(pipelineType_));
		cameraBuffer.SetCommand(commandList, cameraBuffer.GetRootParameterIndex(pipelineType_));

		if (modelData_.meshes[meshIndex].material.textureName) {
			NewMoon::SetGraphicsRootDescriptorTable(commandList, 2, modelData_.meshes[meshIndex].material.textureName.value());
		}
		commandList->DrawIndexedInstanced(static_cast<UINT>(indices_[meshIndex].data.size()), 1, 0, 0, 0);
	}
}
void Model::SkinningAnimationDraw(WorldTransform transform, std::vector<MaterialObject3D>& materials,
	const std::string& animationName, BlendMode blendMode) {

	auto commandList = NewMoon::GetCommandList();
	auto cameraBuffer = NewMoonGame::GetGameCamera()->GetCameraBuffer();
	auto lightBuffer = NewMoonGame::GetGameLight()->GetLightBuffer();

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

		materials[meshIndex].SetCommand(commandList, materials[meshIndex].GetRootParameterIndex());
		transform.SetCommand(commandList, transform.GetRootParameterIndex());
		lightBuffer.SetCommand(commandList, lightBuffer.GetRootParameterIndex(pipelineType_));
		cameraBuffer.SetCommand(commandList, cameraBuffer.GetRootParameterIndex(pipelineType_));

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
void Model::DrawWave(WorldTransform transform, std::vector<MaterialObject3D>& materials,
	WaveBuffer waveBuffer, BlendMode blendMode) {

	auto commandList = NewMoon::GetCommandList();
	auto cameraBuffer = NewMoonGame::GetGameCamera()->GetCameraBuffer();
	auto lightBuffer = NewMoonGame::GetGameLight()->GetLightBuffer();

	NewMoon::SetGraphicsPipeline(commandList, WaveBort, blendMode);
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList->IASetVertexBuffers(0, 1, &vertices_.front().GetVertexBuffer());
	commandList->IASetIndexBuffer(&indices_.front().GetIndexBuffer());

	materials.front().SetCommand(commandList, materials.front().GetRootParameterIndex());
	transform.SetCommand(commandList, transform.GetRootParameterIndex());
	lightBuffer.SetCommand(commandList, 3);
	cameraBuffer.SetCommand(commandList, 4);
	waveBuffer.SetCommand(commandList, waveBuffer.GetRootParameterIndex());

	NewMoon::SetGraphicsRootDescriptorTable(commandList, 2, "waveBase");
	NewMoon::SetGraphicsRootDescriptorTable(commandList, 6, "waveBlue");
	NewMoon::SetGraphicsRootDescriptorTable(commandList, 7, "waveWhite2");
	commandList->DrawIndexedInstanced(static_cast<UINT>(indices_.front().data.size()), 1, 0, 0, 0);
}
void Model::SetComputeCommands(const std::string& animationName) {

	auto commandList = NewMoon::GetCommandList();

	for (uint32_t meshIndex = 0; meshIndex < meshNum_; ++meshIndex) {

		NewMoon::SetComputePipeline(commandList, ComputePipelineType::SkinningCS);
		commandList->SetComputeRootDescriptorTable(0,
			NewMoonGame::GetModelMangager()->GetSkinClusterData(animationName).paletteSrvHandle.second);
		commandList->SetComputeRootDescriptorTable(1, inputVertices_[meshIndex].GetGpuHandle());
		commandList->SetComputeRootDescriptorTable(2,
			NewMoonGame::GetModelMangager()->GetSkinClusterData(animationName).influenceSrvHandle.second);
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

/*////////////////////////////////////////////////////////////////////////////////
*									Setter
////////////////////////////////////////////////////////////////////////////////*/
void Model::SetTexture(const std::string& textureName) {

	for (uint32_t meshIndex = 0; meshIndex < meshNum_; ++meshIndex) {

		modelData_.meshes[meshIndex].material.textureName = textureName;
	}
}