#pragma once

// MainObject
#include "PipelineStateStructure.h"
#include "VertexObject.h"
#include "WorldTransform.h"
#include "MaterialObject.h"
#include "LightObject.h"
#include "CameraObject.h"

// c++
#include <vector>
#include <array>

// 三角形の頂点数 
static const UINT kTriangleVertexNum = 3;

// defaultVertices
// 反時計回り これで表面
const std::array<Vector3, kTriangleVertexNum> triangleVertices = {
	Vector3(-0.5f,-0.5f,0.0f),
	Vector3(0.0f,0.5f,0.0f),
	Vector3(0.5f,-0.5f,0.0f),
};

/*////////////////////////////////////////////////////////////////////////////////
*								Triangle Class
////////////////////////////////////////////////////////////////////////////////*/
class Triangle {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	Triangle() = default;
	~Triangle() = default;

	// Main
	void Initialize(const std::string& textureName);
	void Update(const std::array<Vector3, kTriangleVertexNum>& vertices);
	void Draw(PipelineType drawType, BlendMode blendMode = BlendMode::kBlendModeNormal);

	// Setter
	template <typename... Args>
	void SetConstBuffers(Args&&... args);

private:
	//===================================================================*/
	//							private Variables
	//===================================================================*/

	VertexObject<VertexData3D> vertex_;
	std::string textureName_;

	WorldTransform worldTransform_;
	MaterialObject3D material_;
	std::vector<LightObject> light_;
	std::vector<CameraObject> camera_;

private:
	//===================================================================*/
	//							private Functions
	//===================================================================*/

	template<typename T>
	void SetConstBuffer(DXConstBuffer<T>& buffer);

};

///===============================================================================
/// template関数定義

template<typename ...Args>
inline void Triangle::SetConstBuffers(Args && ...args) {

	(SetConstBuffer(std::forward<Args>(args)), ...);
}

template<typename T>
inline void Triangle::SetConstBuffer(DXConstBuffer<T>& buffer) {

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