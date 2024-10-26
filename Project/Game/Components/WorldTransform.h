#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Game/Components/BaseTransform.h"

/*////////////////////////////////////////////////////////////////////////////////
*								WorldTransform Class
////////////////////////////////////////////////////////////////////////////////*/
class WorldTransform :
	public BaseTransform {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	WorldTransform() = default;
	~WorldTransform() = default;

	void Update(const Matrix4x4& viewPro) override;

	// Getter
	Vector3 GetWorldPos() const override;

public:
	//===================================================================*/
	//							public Variables
	//===================================================================*/

	// 親
	const WorldTransform* parent_ = nullptr;

};