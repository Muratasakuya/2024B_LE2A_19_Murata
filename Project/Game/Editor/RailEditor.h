#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Game/Components/WorldTransform.h"
#include "Game/Components/MaterialObject.h"
#include "Game/3D/Sphere.h"
#include "Game/Objects/Rail.h"
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

	void ImGui();

	//* catmull-Rom *//

	const size_t segmentCount_ = 128; // 線分の数

	Vector3 Interpolation(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t);
	Vector3 SetCatmullRomPos(const std::vector<Vector3>& points, float t);
	void DrawRailLine();

	//* getter *//

	const std::vector<Vector3>& GetControlPoints() const;

private:
	//===================================================================*/
	//							private Variables
	//===================================================================*/

	//* rail *//
	
	float railSize_;                         // 大きさ
	std::vector<Vector3> railPoints_;        // 制御座標
	std::vector<Vector3> defaultRailPoints_; // デバッグ用
	std::unique_ptr<Rail> rail_;             // モデル

	//* catmull-Rom *//
	
	std::vector<std::pair<std::unique_ptr<Sphere>, WorldTransform>> spheres_; // 表示用球
	const std::string sphereTexture_ = "white";                               // 球テクスチャ
	const std::string railTextureName_ = "white";                             // レールテクスチャ
	MaterialObject3D sphereMaterial_;                                         // 共通マテリアル
	const float sphereScale_ = 0.05f;                                         // 表示用の球のS
	Vector3 spherePos_;                                                       // 調整用座標

	//* json *//

	const std::string railPointsJsonPath_ = "railPoints.json"; // JsonPath
	const std::string railPointsLabel_ = "railPoints";         // railpointsLebel

private:
	//===================================================================*/
	//							private Functions
	//===================================================================*/

	void CreateRailModel();

	void SaveRailPoints();
	void LoadRailPoints();

};