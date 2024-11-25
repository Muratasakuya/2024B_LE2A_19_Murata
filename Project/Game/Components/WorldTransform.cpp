#include "WorldTransform.h"

#include "Engine/Base/NewMoon.h"
#include "Engine/Base/NewMoonGame.h"

/*////////////////////////////////////////////////////////////////////////////////
*						WorldTransform classMethods
////////////////////////////////////////////////////////////////////////////////*/

void WorldTransform::Update() {

	Matrix4x4 worldMatrix =
		Matrix4x4::MakeAxisAffineMatrix(scale, rotation, translation);
	if (parent_) {

		worldMatrix = Matrix4x4::Multiply(worldMatrix, parent_->matrix.World);
	}
	Matrix4x4 wvpMatrix = worldMatrix * NewMoonGame::GameCamera()->GetCamera3D()->GetViewProjectionMatrix();
	Matrix4x4 worldInverseTranspose = Matrix4x4::Transpose(Matrix4x4::Inverse(worldMatrix));

	matrix.World = worldMatrix;
	matrix.WVP = wvpMatrix;
	matrix.WorldInverseTranspose = worldInverseTranspose;

	DXConstBuffer::TransferData(matrix);

}

Vector3 WorldTransform::GetWorldPos() const {

	return BaseTransform::GetWorldPos();
}