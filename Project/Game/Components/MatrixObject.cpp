#include "MatrixObject.h"

#include "Engine/Base/NewMoon.h"
#include "Engine/Base/NewMoonGame.h"

void MatrixObject::Init() {

	// 定数バッファ生成
	DXConstBuffer::Init(NewMoon::GetDXDevice());
}

void MatrixObject::Update(const Transform2D& transform2D) {

	// SRT
	Vector3 scale = { transform2D.size.x,transform2D.size.y,1.0f };
	Vector3 rotate = { 0.0f,0.0f,transform2D.rotate };
	Vector3 translate = { transform2D.pos.x,transform2D.pos.y,0.0f };

	Matrix4x4 worldMatrix =
		Matrix4x4::MakeAffineMatrix(scale, rotate, translate);
	matrix_.WVP = worldMatrix * NewMoonGame::GetGameCamera()->GetViewProjection(CameraType::Orthographic);

	// 定数バッファにデータを転送
	TransferData(matrix_);
}