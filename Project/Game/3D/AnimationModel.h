#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Engine/MyDirectXClass/Pipeline/PipelineStateStructure.h"
#include "Game/3D/Base/BaseModel.h"
#include "Game/Components/AnimationTransform.h"
#include "Game/Components/MaterialObject.h"

// c++
#include <vector>
#include <string>

/*////////////////////////////////////////////////////////////////////////////////
*							AnimationModel Class
////////////////////////////////////////////////////////////////////////////////*/
class AnimationModel :
	public BaseModel {
public:
	//===================================================================*/
	//							public Methods
	//===================================================================*

	AnimationModel() = default;
	~AnimationModel() = default;

	void Init(const std::string& modelName, const std::string& animationName);
	void Draw(AnimationTransform transform, MaterialObject3D material, BlendMode blendMode = BlendMode::kBlendModeNormal);

	void SetAnimationName(const std::string& animationName);

private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

	std::string animationName_;

	//===================================================================*/
	/// Compute
	// IA
	InputVertexObject inputVertices_;
	OutputVertexObject outputVertices_;
	// SkinningInfo
	SkinningInfoData skinningInfoDates_;

private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

	void SetComputeCommands(const std::string& animationName);

};