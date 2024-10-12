#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Game/Components/WorldTransform.h"
#include "Game/Components/MaterialObject.h"
#include "Game/3D/Sphere.h"
#include "Lib/Structure.h"

// c++
#include <memory>
#include <vector>
#include <utility>

/*////////////////////////////////////////////////////////////////////////////////
*							RailEditor Class
////////////////////////////////////////////////////////////////////////////////*/
class RailEditor {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	RailEditor() = default;
	~RailEditor() = default;

	void Init();
	void Update();
	void Draw();

private:
	//===================================================================*/
	//							private Variables
	//===================================================================*/

	// レールの横幅と縦幅
	Vector2 railSize_;
	// レール座標
	std::vector<Vector3> railPoints_;

	// スプライン曲線の頂点表示用
	std::vector<std::pair<std::unique_ptr<Sphere>, WorldTransform>> spheres_;
	// 使用するテクスチャ
	const std::string sphereTexture_ = "white";
	MaterialObject3D sphereMaterial_;
	const float sphereScale_ = 0.05f;
	Vector3 spherePos_;

private:
	//===================================================================*/
	//							private Functions
	//===================================================================*/

	void SetCatmullRomVertices();

};