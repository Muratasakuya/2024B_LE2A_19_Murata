#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Game/3D/Base/BaseModel.h"

/*////////////////////////////////////////////////////////////////////////////////
*								Player Class
////////////////////////////////////////////////////////////////////////////////*/
class Player :
	public BaseModel {
public:
	//===================================================================*/
	//							public Function
	//===================================================================*/

	Player() = default;
	~Player() = default;

	void Init();
	void Update(const Matrix4x4& viewPro);
	void Draw();

	void ImGui();

	// Setter
	void SetForward(const Vector3& forward);
	void SetParent(const WorldTransform* parent);

public:
	//===================================================================*/
	//							private variable
	//===================================================================*/

	// 前方ベクトル
	Vector3 forward_;

};