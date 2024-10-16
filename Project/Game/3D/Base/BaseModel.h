#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Engine/MyDirectXClass/Pipeline/PipelineStateStructure.h"
#include "Game/Components/WorldTransform.h"
#include "Game/Components/MaterialObject.h"
#include "Game/Components/WaveBuffer.h"
#include "Game/3D/Model.h"

// c++
#include <memory>
#include <vector>

/*////////////////////////////////////////////////////////////////////////////////
*							BaseModel Class
////////////////////////////////////////////////////////////////////////////////*/
class BaseModel {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	BaseModel() = default;
	virtual ~BaseModel() = default;

protected:
	//===================================================================*/
	//							protected Methods
	//===================================================================*/

	//===================================================================*/
	/// Variables

	std::unique_ptr<Model> model_;

	WorldTransform transform_;
	std::vector<MaterialObject3D> materials_;

	//===================================================================*/
	/// Functions

	void Init(const std::string& modelName);
	void Update();
	void Draw(BlendMode blendMode = BlendMode::kBlendModeNormal);

};

