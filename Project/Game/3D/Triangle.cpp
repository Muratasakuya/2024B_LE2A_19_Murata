#include "Triangle.h"

#include "Engine/Base/NewMoon.h"
#include "Engine/Base/NewMoonGame.h"
#include "Engine/Managers/DXConstBufferManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*									Main
////////////////////////////////////////////////////////////////////////////////*/
void Triangle::Init(const std::string& textureName) {

	// 使用するテクスチャ
	textureName_ = textureName;

	// ConstBuffer初期化
	vertex_.Init(kTriangleVertexNum);
	vertex_.data.resize(kTriangleVertexNum);
}

void Triangle::Update(const std::array<Vector3, kTriangleVertexNum>& vertices) {

	// 頂点
	vertex_.data[0].pos = { vertices[0].x,vertices[0].y ,vertices[0].z,1.0f };
	vertex_.data[1].pos = { vertices[1].x,vertices[1].y ,vertices[1].z,1.0f };
	vertex_.data[2].pos = { vertices[2].x,vertices[2].y ,vertices[2].z,1.0f };

	// UV 基本固定
	vertex_.data[0].texcoord = { 0.0f,1.0f };
	vertex_.data[1].texcoord = { 0.5f,0.0f };
	vertex_.data[2].texcoord = { 1.0f,1.0f };

	// 法線
	Vector3 normal = Vector3::CalculateTriangleNormal(
		{ vertices[0].x,vertices[0].y ,vertices[0].z,1.0f },
		{ vertices[1].x,vertices[1].y ,vertices[1].z,1.0f },
		{ vertices[2].x,vertices[2].y ,vertices[2].z,1.0f }
	);
	for (uint32_t i = 0; i < kTriangleVertexNum; i++) {

		vertex_.data[i].normal = normal;
	}

	// ConstBuffer転送
	vertex_.Update();
}

void Triangle::Draw(BlendMode blendMode) {

	auto commandList = NewMoon::GetCommandList();
	auto cameraBuffer = NewMoonGame::GetGameCamera()->GetCameraBuffer();
	auto lightBuffer = NewMoonGame::GetGameLight()->GetLightBuffer();
	DXConstBufferManager constBuffer;

	NewMoon::SetGraphicsPipeline(commandList, pObject3D, blendMode);
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList->IASetVertexBuffers(0, 1, &vertex_.GetVertexBuffer());
	constBuffer.SetCommands(commandList, pObject3D, worldTransform_, material_, lightBuffer, cameraBuffer);
	NewMoon::SetGraphicsRootDescriptorTable(commandList, 2, textureName_);
	commandList->DrawInstanced(kTriangleVertexNum, 1, 0, 0);
}