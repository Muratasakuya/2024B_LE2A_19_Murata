#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Engine/MyDirectXClass/Pipeline/PipelineStateStructure.h"
#include "Game/3D/Base/BaseModel.h"
#include "Game/Components/WorldTransform.h"
#include "Game/Components/MaterialObject.h"

// c++
#include <vector>
#include <string>

/*////////////////////////////////////////////////////////////////////////////////
*									Model Class
////////////////////////////////////////////////////////////////////////////////*/
class Model :
	public BaseModel {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*

	Model() = default;
	~Model() = default;

	void Draw(WorldTransform transform, std::vector<MaterialObject3D>& materials,
		BlendMode blendMode = BlendMode::kBlendModeNormal);

	// Setter
	void SetTexture(const std::string& textureName);

	// Getter
	size_t GetMeshNum() const;

};