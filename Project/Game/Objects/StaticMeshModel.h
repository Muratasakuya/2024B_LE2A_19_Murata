#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Game/Components/StaticMeshBuffer.h"
#include "Game/Components/TriangleVertexBuffer.h"
#include "Game/Components/InputAssembler.h"
#include "Lib/Structure.h"

/*////////////////////////////////////////////////////////////////////////////////
*							StaticMeshModel Class
////////////////////////////////////////////////////////////////////////////////*/
class StaticMeshModel {
public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	StaticMeshModel() = default;
	~StaticMeshModel() = default;

	void Init();

	void Update();

	void Draw();

	void ImGui();

	//* getter *//

	std::string GetName() const { return name_; }

private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

	//===================================================================*/
	///* enum class

	enum class AnimationState {

		Begin, //* 始まり
		Play,  //* アニメーション中
		End,   //* 待ち時間
	};

	struct Timer {

		float elapsed; //* 経過時間
		float target;  //* 目標時間
	};

	//===================================================================*/
	///* variables

	//* common *//

	std::string textureName_;
	ModelData modelData_;
	InputAssembler inputAssembler_;
	int numInstance_;
	int prevNumInstance_;

	std::vector<Transform3D> transforms_;
	Vector4 color_;

	std::vector<Vector3> triangleNormals_;

	std::string name_;

	//* animation *//

	AnimationState animationState_;

	bool beginMoveOnce_;

	Timer beginWaitTimer_;
	Timer beginTimer_;

	Timer playTimer_;

	Timer endTimer_;

	bool playMoveOnce_;

	std::vector<Vector3> velocity_;
	std::vector<Vector3> acceleration_;

	//* meshBuffer *//

	uint32_t instancingMeshSrvIndex_;
	StaticMeshBuffer meshBuffer_;

	//* triangleBuffer *//

	uint32_t instancingTriangleSrvIndex_;
	std::vector<TriangleDataForGPU> triangleData_;
	std::vector<Vector4> trianglePositions_;
	TriangleVertexBuffer triangleVertexBuffer_;

	//===================================================================*/
	///* functions

	void CreateVertexData(const std::string& name);

};