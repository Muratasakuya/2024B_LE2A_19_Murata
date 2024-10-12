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
	void Draw(BlendMode blendMode = BlendMode::kBlendModeNormal);

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

private:
	//===================================================================*/
	//							private Functions
	//===================================================================*/

	void VertexSetting();
	void IndexSetting();

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
	}
}

///===============================================================================