#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Game/Components/VertexObject.h"
#include "Game/Components/MaterialObject.h"
#include "Game/Components/CameraObject.h"
#include "Lib/Structure.h"
#include "Lib/Math/Physics.h"

// c++
#include <unordered_map>

/*////////////////////////////////////////////////////////////////////////////////
*								PrimitiveDrawer Class
////////////////////////////////////////////////////////////////////////////////*/
class PrimitiveDrawer {
private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

	// 線分の最大数
	static const uint32_t kMaxLineCount_ = 2048;
	// 線分の頂点数
	static const uint32_t kVertexCountLine_ = 2;

public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	PrimitiveDrawer() = default;
	~PrimitiveDrawer() = default;

	// Main
	void Init(const ViewProjectionBuffer& viewProBuffer);
	void Update();

	void DrawLine2D(const Vector2& pointA, const Vector2& pointB, const LineColor& color);
	void DrawLine3D(const Vector3& pointA, const Vector3& pointB, const LineColor& color);

	void DrawGrid();

	void DrawSphere(float radius, const Vector3& centerPos, const LineColor& color);

	void DrawOBB(const Vector3& size,const Quaternion& rotate, const Vector3& centerPos, const LineColor& color);

	void DrawAABB(const AABBInfo& aabb,const LineColor& color);

	void Reset();

private:
	//===================================================================*/
	//							private Variables
	//===================================================================*/

	PrimitiveVertexObject vertex_;

	std::unordered_map<LineColor, PrimitiveMaterial> lineMaterials_;

	ViewProjectionBuffer viewProBuffer_;

	// 線分使用インデックス
	uint32_t indexLine_;

};