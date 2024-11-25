#include "BaseModel.h"

#include "Engine/Base/NewMoonGame.h"

/*////////////////////////////////////////////////////////////////////////////////
*						BaseModel classMethods
////////////////////////////////////////////////////////////////////////////////*/

void BaseModel::Init(const std::string& modelName) {

	modelData_ = NewMoonGame::GetModelManager()->GetModelData(modelName);
	meshNum_ = modelData_.meshes.size();

	for (uint32_t index = 0; index < meshNum_; ++index) {

		inputAssembler_.Init(modelData_.meshes[index]);
	}

	setPipelineType_ = std::nullopt;

}

void BaseModel::SetPipelineType(const PipelineType& pipelineType) {

	setPipelineType_ = pipelineType;
}
