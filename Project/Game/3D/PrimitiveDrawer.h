#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Game/Components/VertexObject.h"
#include "Game/Components/MaterialObject.h"
#include "Game/Components/CameraObject.h"
#include "Lib/Structure.h"

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
	void DrawLine(const Vector3& pointA, const Vector3& pointB, const Vector4& color);

	void DrawGrid(); // 平面描画

	void Reset();

private:
	//===================================================================*/
	//							private Variables
	//===================================================================*/

	PrimitiveVertexObject vertex_;
	PrimitiveMaterial material_;

	ViewProjectionBuffer viewProBuffer_;

	// 線分使用インデックス
	uint32_t indexLine_;

};