#include "PrimitiveDrawer.h"

#include "Engine/Base/NewMoon.h"

/*////////////////////////////////////////////////////////////////////////////////
*									Main
////////////////////////////////////////////////////////////////////////////////*/
void PrimitiveDrawer::Init(const ViewProjectionBuffer& viewProBuffer) {

	viewProBuffer_ = viewProBuffer;

	// ConstBuffer初期化
	vertex_.Init(kMaxLineCount_ * kVertexCountLine_);
	material_.Init();
	material_.color.Init();

	// 転送用の頂点数でresize
	vertex_.pos.resize(kMaxLineCount_ * kVertexCountLine_);
}

void PrimitiveDrawer::Update() {

	// ConstBuffer転送
	vertex_.Update();
	material_.Update();
}

void PrimitiveDrawer::DrawLine(const Vector3& pointA, const Vector3& pointB, const Vector4& color) {

	assert(indexLine_ < kMaxLineCount_);

	auto commandList = NewMoon::GetCommandList();

	vertex_.pos[indexLine_] = { pointA.x,pointA.y,pointA.z,1.0f };
	indexLine_++;
	vertex_.pos[indexLine_] = { pointB.x,pointB.y,pointB.z,1.0f };
	material_.color = color;
	indexLine_++;

	NewMoon::SetGraphicsPipeline(commandList, PrimitiveLine, kBlendModeNormal);
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	commandList->IASetVertexBuffers(0, 1, &vertex_.GetVertexBuffer());
	material_.SetCommand(commandList, 0);
	viewProBuffer_.SetCommand(commandList, viewProBuffer_.GetRootParameterIndex());
	commandList->DrawInstanced(kVertexCountLine_, 1, indexLine_ - kVertexCountLine_, 0);
}

void PrimitiveDrawer::DrawGrid() {

	const uint32_t kSubdivision = 24;
	const float kGridHalfWidth = 5.0f;
	const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision);

	const Vector4 kLineColor = { 1.0f, 1.0f, 1.0f, 1.0f };

	for (uint32_t index = 0; index <= kSubdivision; ++index) {

		float offset = -kGridHalfWidth + index * kGridEvery;

		// 横
		Vector3 verticalStart(offset, 0.0f, kGridHalfWidth);
		Vector3 verticalEnd(offset, 0.0f, -kGridHalfWidth);
		DrawLine(verticalStart, verticalEnd, kLineColor);

		// 縦
		Vector3 horizontalStart(-kGridHalfWidth, 0.0f, offset);
		Vector3 horizontalEnd(kGridHalfWidth, 0.0f, offset);
		DrawLine(horizontalStart, horizontalEnd, kLineColor);
	}
}

void PrimitiveDrawer::Reset() {

	indexLine_ = 0;
}