#pragma once

// MainObject
#include "PipelineStateStructure.h"
#include "VertexObject.h"
#include "IndexObject.h"
#include "WorldTransform.h"
#include "MaterialObject.h"
#include "LightObject.h"
#include "CameraObject.h"

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
	void Initialize(const std::string& modelName);
	void Draw(BlendMode blendMode = BlendMode::kBlendModeNormal);
	void SkinningAnimationDraw(const std::string& animationName, BlendMode blendMode = BlendMode::kBlendModeNormal);

	// Setter
	template <typename... Args>
	void SetConstBuffers(Args&&... args);

	// Getter
	size_t GetMeshNum() const;

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

	std::vector<WorldTransform> worldTransform_;
	std::vector<MaterialObject3D> material_;
	std::vector<LightObject> light_;
	std::vector<CameraObject> camera_;

	//===================================================================*/
	/// Compute
	// Input Output
	std::vector<InputVertexObject> inputVertices_;
	std::vector<OutputVertexObject> outputVertices_;
	// SkinningInfo
	std::vector<SkinningInfoData> skinningInfoDatas_;

private:
	//===================================================================*/
	//							private Functions
	//===================================================================*/

	template<typename T>
	void SetConstBuffer(DXConstBuffer<T>& buffer);
	void SetCompute(const std::string& animationName);

};

///===============================================================================
/// template関数定義

template<typename ...Args>
inline void Model::SetConstBuffers(Args && ...args) {

	(SetConstBuffer(std::forward<Args>(args)), ...);
}

template<typename T>
inline void Model::SetConstBuffer(DXConstBuffer<T>& buffer) {

	if constexpr (std::is_same_v<T, TransformationMatrix>) {

		worldTransform_.push_back(static_cast<WorldTransform&>(buffer));
	} else if constexpr (std::is_same_v<T, Material3D>) {

		material_.push_back(static_cast<MaterialObject3D&>(buffer));
	} else if constexpr (std::is_same_v<T, PunctualLight>) {

		light_.push_back(static_cast<LightObject&>(buffer));
	} else if constexpr (std::is_same_v<T, CameraForGPU>) {

		camera_.push_back(static_cast<CameraObject&>(buffer));
	}
}

///===============================================================================