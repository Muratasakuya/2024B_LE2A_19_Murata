#include "BaseModel.h"

#include "Engine/Base/NewMoonGame.h"

/*////////////////////////////////////////////////////////////////////////////////
*						BaseModel classMethods
////////////////////////////////////////////////////////////////////////////////*/

void BaseModel::Init(const std::string& modelName) {

	modelData_ = NewMoonGame::GetModelMangager()->GetModelData(modelName);
	meshNum_ = modelData_.meshes.size();

	inputAssembler_.verticesNum.resize(meshNum_);
	inputAssembler_.indicesNum.resize(meshNum_);
	inputAssembler_.vertices.resize(meshNum_);
	inputAssembler_.indices.resize(meshNum_);

	//* Init Transfer *//
	for (uint32_t index = 0; index < meshNum_; ++index) {

		const auto& mesh = modelData_.meshes[index];
		inputAssembler_.verticesNum[index] = static_cast<UINT>(mesh.vertices.size());
		inputAssembler_.indicesNum[index] = static_cast<UINT>(mesh.indices.size());

		inputAssembler_.vertices[index].Init(inputAssembler_.verticesNum[index]);
		inputAssembler_.indices[index].Init(inputAssembler_.indicesNum[index]);

		// modelMeshData -> IA
		inputAssembler_.vertices[index].data.resize(inputAssembler_.verticesNum[index]);
		std::copy(mesh.vertices.begin(), mesh.vertices.end(), inputAssembler_.vertices[index].data.begin());
		inputAssembler_.indices[index].data.resize(inputAssembler_.indicesNum[index]);
		std::copy(mesh.indices.begin(), mesh.indices.end(), inputAssembler_.indices[index].data.begin());

		inputAssembler_.vertices[index].Update();
		inputAssembler_.indices[index].Update();
	}

	// ここでnullopt
	setPipelineType_ = std::nullopt;
}

void BaseModel::SetPipelineType(const PipelineType& pipelineType) {

	setPipelineType_ = pipelineType;
}
