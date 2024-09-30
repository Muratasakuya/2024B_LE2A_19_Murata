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
	void Initialize(const std::string& textureName);
	void Draw(PipelineType drawType, BlendMode blendMode = BlendMode::kBlendModeNormal);

	// Setter
	template <typename... Args>
	void SetConstBuffers(Args&&... args);

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

	WorldTransform worldTransform_;
	MaterialObject3D material_;
	std::vector<LightObject> light_;
	std::vector<CameraObject> camera_;

private:
	//===================================================================*/
	//							private Functions
	//===================================================================*/

	void VertexUpdate();
	void IndexUpdate();

	template<typename T>
	void SetConstBuffer(DXConstBuffer<T>& buffer);

};

///===============================================================================
/// template関数定義

template<typename ...Args>
inline void Sphere::SetConstBuffers(Args && ...args) {

	(SetConstBuffer(std::forward<Args>(args)), ...);
}

template<typename T>
inline void Sphere::SetConstBuffer(DXConstBuffer<T>& buffer) {

	if constexpr (std::is_same_v<T, TransformationMatrix>) {

		worldTransform_ = static_cast<WorldTransform&>(buffer);
	} else if constexpr (std::is_same_v<T, Material3D>) {

		material_ = static_cast<MaterialObject3D&>(buffer);
	} else if constexpr (std::is_same_v<T, PunctualLight>) {

		light_.push_back(static_cast<LightObject&>(buffer));
	} else if constexpr (std::is_same_v<T, CameraForGPU>) {

		camera_.push_back(static_cast<CameraObject&>(buffer));
	}
}

///===============================================================================