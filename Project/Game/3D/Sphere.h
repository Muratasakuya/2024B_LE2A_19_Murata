#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Engine/MyDirectXClass/Pipeline/PipelineStateStructure.h"
#include "Game/Components/VertexObject.h"
#include "Game/Components/IndexObject.h"
#include "Game/Components/WorldTransform.h"
#include "Game/Components/MaterialObject.h"
#include "Game/Components/LightObject.h"
#include "Game/Components/CameraObject.h"

// c++
#include <vector>

/*////////////////////////////////////////////////////////////////////////////////
*								Sphere Class
////////////////////////////////////////////////////////////////////////////////*/
class Sphere {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*

	Sphere() = default;
	~Sphere() = default;

	// Main
	void Init(const std::string& textureName);
	void Draw(WorldTransform transform, MaterialObject3D material, BlendMode blendMode = BlendMode::kBlendModeNormal);

private:
	//===================================================================*/
	//							private Variables
	//===================================================================*/

	// default
	uint32_t subDivision_ = 16;
	UINT vertexNum_ = subDivision_ * subDivision_ * 4;
	UINT indexNum_ = subDivision_ * subDivision_ * 6;

	VertexObject<VertexData3D> vertex_;
	IndexObject index_;
	std::string textureName_;

private:
	//===================================================================*/
	//							private Functions
	//===================================================================*/

	void VertexSetting();
	void IndexSetting();

};