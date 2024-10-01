#include "Sphere.h"

#include "Engine/Base/NewMoon.h"
#include "Engine/Managers/DXConstBufferManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*									Main
////////////////////////////////////////////////////////////////////////////////*/
void Sphere::Initialize(const std::string& textureName) {

	// 使用するテクスチャ
	textureName_ = textureName;

	// ConstBuffer初期化
	vertex_.Initialize(vertexNum_);
	index_.Initialize(indexNum_);

	for (uint32_t i = 0; i < vertexNum_; i++) {

		VertexData3D vertexData{};
		vertex_.data.push_back(vertexData);
	}
	for (uint32_t i = 0; i < indexNum_; i++) {

		uint32_t indexData;
		index_.data.push_back(indexData);
	}

	// 頂点設定
	VertexSetting();
	IndexSetting();

	// ConstBuffer転送
	vertex_.Update();
	index_.Update();
}

void Sphere::Draw(BlendMode blendMode) {

	auto commandList = NewMoon::GetCommandList();
	DXConstBufferManager constBuffer;

	NewMoon::SetGraphicsPipeline(commandList, pObject3D, blendMode);
	commandList->IASetVertexBuffers(0, 1, &vertex_.GetVertexBuffer());
	commandList->IASetIndexBuffer(&index_.GetIndexBuffer());
	constBuffer.SetCommands(commandList, pObject3D, worldTransform_, material_, light_);
	NewMoon::SetGraphicsRootDescriptorTable(commandList, 2, textureName_);
	commandList->DrawIndexedInstanced(indexNum_, 1, 0, 0, 0);
}

/*////////////////////////////////////////////////////////////////////////////////
*									頂点情報
////////////////////////////////////////////////////////////////////////////////*/
void Sphere::VertexSetting() {

	// 経度分割1つ分の角度
	const float kLonEvery = std::numbers::pi_v<float> *2.0f / float(subDivision_);
	// 緯度分割1つ分の角度
	const float kLatEvery = std::numbers::pi_v<float> / float(subDivision_);

	// 緯度方向に分割
	for (uint32_t latIndex = 0; latIndex < subDivision_; latIndex++) {

		float lat = -std::numbers::pi_v<float> / 2.0f + kLatEvery * latIndex;

		// 経度の方向に分割
		for (uint32_t lonIndex = 0; lonIndex < subDivision_; lonIndex++) {

			uint32_t start = (latIndex * subDivision_ + lonIndex) * 4;
			float lon = lonIndex * kLonEvery + std::numbers::pi_v<float> / 2.0f;

			float u0 = static_cast<float>(lonIndex) / subDivision_;
			float u1 = static_cast<float>(lonIndex + 1) / subDivision_;
			float v0 = 1.0f - static_cast<float>(latIndex) / subDivision_;
			float v1 = 1.0f - static_cast<float>(latIndex + 1) / subDivision_;

			/*=========================================================================================*/
			// 各頂点の値を書き込む
			// 基準点 a
			vertex_.data[start].pos.x =
				std::cos(lat) * std::cos(lon);

			vertex_.data[start].pos.y =
				std::sin(lat);

			vertex_.data[start].pos.z =
				std::cos(lat) * std::sin(lon);

			vertex_.data[start].pos.w = 1.0f;

			vertex_.data[start].texcoord = { u0, v0 };

			vertex_.data[start].normal.x = vertex_.data[start].pos.x;
			vertex_.data[start].normal.y = vertex_.data[start].pos.y;
			vertex_.data[start].normal.z = vertex_.data[start].pos.z;

			/*=========================================================================================*/
			// 基準点 b
			vertex_.data[start + 1].pos.x =
				std::cos(lat + kLatEvery) * std::cos(lon);

			vertex_.data[start + 1].pos.y =
				std::sin(lat + kLatEvery);

			vertex_.data[start + 1].pos.z =
				std::cos(lat + kLatEvery) * std::sin(lon);

			vertex_.data[start + 1].pos.w = 1.0f;

			vertex_.data[start + 1].texcoord = { u0, v1 };

			vertex_.data[start + 1].normal.x = vertex_.data[start + 1].pos.x;
			vertex_.data[start + 1].normal.y = vertex_.data[start + 1].pos.y;
			vertex_.data[start + 1].normal.z = vertex_.data[start + 1].pos.z;

			/*=========================================================================================*/
			// 基準点 c
			vertex_.data[start + 2].pos.x =
				std::cos(lat) * std::cos(lon + kLonEvery);

			vertex_.data[start + 2].pos.y =
				std::sin(lat);

			vertex_.data[start + 2].pos.z =
				std::cos(lat) * std::sin(lon + kLonEvery);

			vertex_.data[start + 2].pos.w = 1.0f;

			vertex_.data[start + 2].texcoord = { u1, v0 };

			vertex_.data[start + 2].normal.x = vertex_.data[start + 2].pos.x;
			vertex_.data[start + 2].normal.y = vertex_.data[start + 2].pos.y;
			vertex_.data[start + 2].normal.z = vertex_.data[start + 2].pos.z;

			/*=========================================================================================*/
			// 基準点 d
			vertex_.data[start + 3].pos.x =
				std::cos(lat + kLatEvery) * std::cos(lon + kLonEvery);

			vertex_.data[start + 3].pos.y =
				std::sin(lat + kLatEvery);

			vertex_.data[start + 3].pos.z =
				std::cos(lat + kLatEvery) * std::sin(lon + kLonEvery);

			vertex_.data[start + 3].pos.w = 1.0f;

			vertex_.data[start + 3].texcoord = { u1, v1 };

			vertex_.data[start + 3].normal.x = vertex_.data[start + 3].pos.x;
			vertex_.data[start + 3].normal.y = vertex_.data[start + 3].pos.y;
			vertex_.data[start + 3].normal.z = vertex_.data[start + 3].pos.z;
		}
	}
}

void Sphere::IndexSetting() {

	for (uint32_t latIndex = 0; latIndex <= subDivision_; latIndex++) {
		for (uint32_t lonIndex = 0; lonIndex <= subDivision_; lonIndex++) {
			uint32_t baseIndex = (latIndex * (subDivision_)+lonIndex) * 4;

			if (6 * (latIndex * subDivision_ + lonIndex) == 1536) {
				break;
			}
			index_.data[6 * (latIndex * subDivision_ + lonIndex)] = baseIndex;         // 0
			index_.data[6 * (latIndex * subDivision_ + lonIndex) + 1] = baseIndex + 1; // 1
			index_.data[6 * (latIndex * subDivision_ + lonIndex) + 2] = baseIndex + 2; // 2
			index_.data[6 * (latIndex * subDivision_ + lonIndex) + 3] = baseIndex + 2; // 3
			index_.data[6 * (latIndex * subDivision_ + lonIndex) + 4] = baseIndex + 1; // 4
			index_.data[6 * (latIndex * subDivision_ + lonIndex) + 5] = baseIndex + 3; // 5
		}
	}
}
