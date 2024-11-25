#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Engine/MyDirectXClass/Pipeline/PipelineStateStructure.h"
#include "Game/Components/InputAssembler.h"

// c++
#include <memory>
#include <vector>

/*////////////////////////////////////////////////////////////////////////////////
*							BaseModel Class
////////////////////////////////////////////////////////////////////////////////*/
class BaseModel {
public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	BaseModel() = default;
	virtual ~BaseModel() = default;

	virtual void Init(const std::string& modelName);

	// Setter
	void SetPipelineType(const PipelineType& pipelineType);

protected:
	//===================================================================*/
	//							protected Methods
	//===================================================================*/

	size_t meshNum_;
	ModelData modelData_;

	InputAssembler inputAssembler_;

	PipelineType pipelineType_;
	std::optional<PipelineType> setPipelineType_;

};