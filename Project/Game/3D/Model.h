#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Engine/MyDirectXClass/Pipeline/PipelineStateStructure.h"
#include "Game/Components/VertexObject.h"
#include "Game/Components/IndexObject.h"
#include "Game/Components/WorldTransform.h"
#include "Game/Components/MaterialObject.h"
#include "Game/Components/WaveBuffer.h"

// c++
#include <vector>
#include <string>

/*////////////////////////////////////////////////////////////////////////////////
*									Model Class
////////////////////////////////////////////////////////////////////////////////*/
class Model {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*

	Model() = default;
	~Model() = default;

	// Main
	void Init(const std::string& modelName);
	void Draw(WorldTransform transform, std::vector<MaterialObject3D>& materials,
		BlendMode blendMode = BlendMode::kBlendModeNormal);
	void SkinningAnimationDraw(WorldTransform transform, std::vector<MaterialObject3D>& materials,
		const std::string& animationName, BlendMode blendMode = BlendMode::kBlendModeNormal);

	// Getter
	size_t GetMeshNum() const;

	// Setter
	void SetTexture(const std::string& textureName);

private:
	//===================================================================*/
	//							private Variables
	//===================================================================*/

	size_t meshNum_;
	ModelData modelData_;
	PipelineType pipelineType_;

	std::vector<VertexObject<VertexData3D>> vertices_;
	std::vector<UINT> verticesNum_;

	std::vector<IndexObject> indices_;
	std::vector<UINT> indicesNum_;

	//===================================================================*/
	/// Compute
	// Input Output
	std::vector<InputVertexObject> inputVertices_;
	std::vector<OutputVertexObject> outputVertices_;
	// SkinningInfo
	std::vector<SkinningInfoData> skinningInfoDatas_;

private:
	//===================================================================*/
	//							private Function
	//===================================================================*/

	void SetComputeCommands(const std::string& animationName);

};