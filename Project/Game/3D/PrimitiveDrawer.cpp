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

	NewMoon::SetGraphicsPipeline(commandList, PrimitiveLine, kBlendModeNormal);
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	commandList->IASetVertexBuffers(0, 1, &vertex_.GetVertexBuffer());
	material_.SetCommand(commandList, 0);
	viewProBuffer_.SetCommand(commandList, 1);
	commandList->DrawInstanced(kVertexCountLine_, 1, 0, 0);
	indexLine_++;
}

void PrimitiveDrawer::Reset() {

	indexLine_ = 0;
}