#include "Sprite.h"

#include "Engine/Base/NewMoon.h"

/*////////////////////////////////////////////////////////////////////////////////
*									Main
////////////////////////////////////////////////////////////////////////////////*/
void Sprite::Initialize(const std::string& textureName) {

	// 使用するテクスチャ
	textureName_ = textureName;

	// ConstBuffer初期化
	vertex_.Initialize(kSpriteVertexNum);
	index_.Initialize(kSpriteIndexNum);

	material_.Initialize();
	matrix_.Initialize();

	for (uint32_t i = 0; i < kSpriteVertexNum; i++) {

		VertexData2D vertexData{};
		vertex_.data.push_back(vertexData);
	}
	for (uint32_t i = 0; i < kSpriteIndexNum; i++) {

		uint32_t indexData;
		index_.data.push_back(indexData);
	}

	// Indexの設定
	index_.data[0] = 0;
	index_.data[1] = 1;
	index_.data[2] = 2;
	index_.data[3] = 1;
	index_.data[4] = 3;
	index_.data[5] = 2;

	transform2D_.Initialize();
}

void Sprite::Update() {

	// 頂点更新
	VertexUpdate();

	// ConstBuffer転送
	vertex_.Update();
	index_.Update();

	material_.Update();
	matrix_.Update(transform2D_);
}

void Sprite::Draw(const BlendMode& blendMode) {

	auto commandList = NewMoon::GetCommandList();

	NewMoon::SetGraphicsPipeline(commandList, Object2D, blendMode);
	commandList->IASetVertexBuffers(0, 1, &vertex_.GetVertexBuffer());
	commandList->IASetIndexBuffer(&index_.GetIndexBuffer());
	material_.SetCommand(commandList, 0);
	matrix_.SetCommand(commandList, 1);
	NewMoon::SetGraphicsRootDescriptorTable(commandList, 2, textureName_);
	commandList->DrawIndexedInstanced(kSpriteIndexNum, 1, 0, 0, 0);
}

/*////////////////////////////////////////////////////////////////////////////////
*									頂点情報
////////////////////////////////////////////////////////////////////////////////*/
void Sprite::VertexUpdate() {

	// アンカーポイント
	float left   = 0.0f - transform2D_.anchorPoint.x;
	float right  = 1.0f - transform2D_.anchorPoint.x;
	float top    = 0.0f - transform2D_.anchorPoint.y;
	float bottom = 1.0f - transform2D_.anchorPoint.y;

	// テクスチャのサイズを画像サイズに合わせる
	const DirectX::TexMetadata& metadata = NewMoon::GetMetaData(textureName_);
	transform2D_.textureSize = { static_cast<float>(metadata.width) ,static_cast<float>(metadata.height) };
	transform2D_.size = transform2D_.textureSize;

	// 横
	float texLeft = transform2D_.textureLeftTop.x / static_cast<float>(metadata.width);
	float texRight = (transform2D_.textureLeftTop.x + transform2D_.textureSize.x) / static_cast<float>(metadata.width);
	// 縦
	float texTop = transform2D_.textureLeftTop.y / static_cast<float>(metadata.height);
	float texBottom = (transform2D_.textureLeftTop.y + transform2D_.textureSize.y) / static_cast<float>(metadata.height);

	// 左下
	vertex_.data[0].pos = { left,bottom };
	vertex_.data[0].texcoord = { texLeft,texBottom };
	// 左上
	vertex_.data[1].pos = { left,top };
	vertex_.data[1].texcoord = { texLeft,texTop };
	// 右下
	vertex_.data[2].pos = { right,bottom };
	vertex_.data[2].texcoord = { texRight,texBottom };
	// 右上
	vertex_.data[3].pos = { right,top };
	vertex_.data[3].texcoord = { texRight,texTop };
}

/*////////////////////////////////////////////////////////////////////////////////
*									Setter
////////////////////////////////////////////////////////////////////////////////*/
void Sprite::SetPos(const Vector2& pos) { transform2D_.pos = pos; }
void Sprite::SetAnchor(const Vector2& anchor) { transform2D_.anchorPoint = anchor; }
void Sprite::SetTextureLeftTop(const Vector2& leftTop) { transform2D_.textureLeftTop = leftTop; }
void Sprite::SetTextureSize(const Vector2& textureSize) { transform2D_.textureSize = textureSize; }
void Sprite::SetColor(const Vector4& color) { material_.color = color; }
void Sprite::SetAlpha(float alpha) { material_.color.w = alpha; }