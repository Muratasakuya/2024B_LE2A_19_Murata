#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Engine/MyDirectXClass/Pipeline/PipelineStateStructure.h"
#include "Game/Components/VertexObject.h"
#include "Game/Components/IndexObject.h"
#include "Game/Components/MaterialObject.h"
#include "Game/Components/MatrixObject.h"

// c++
#include <string>

// 頂点数 Vertex
static const UINT kSpriteVertexNum = 4;
// 頂点数 Index
static const UINT kSpriteIndexNum = 6;

/*////////////////////////////////////////////////////////////////////////////////
*									Sprite Class
////////////////////////////////////////////////////////////////////////////////*/
class Sprite {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	// default
	Sprite() = default;
	~Sprite() = default;

	// Main
	void Init(const std::string& textureName);
	void Update();
	void Draw(const BlendMode& blendMode = BlendMode::kBlendModeNormal);

	// Setter
	void SetPos(const Vector2& pos);
	void SetAnchor(const Vector2& anchor);
	void SetCenterAnchor();
	void SetTextureLeftTop(const Vector2& leftTop);
	void SetTextureSize(const Vector2& textureSize);
	void SetOriginalTextureSize(const Vector2& textrueSize);
	void SetColor(const Vector4& color);
	void SetAlpha(float alpha);
	void AjustTextureSize(bool ajustTexture);

private:
	//===================================================================*/
	//							private Variables
	//===================================================================*/

	VertexObject<VertexData2D> vertex_;
	IndexObject index_;
	MaterialObject2D material_;
	MatrixObject matrix_;

	Transform2D transform2D_;

	std::string textureName_;

	bool ajustTexture_ = true;

private:
	//===================================================================*/
	//							private Function
	//===================================================================*/

	void VertexUpdate();

};