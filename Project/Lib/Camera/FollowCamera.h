#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Lib/Structure.h"
#include "Game/Components/WorldTransform.h"

/*////////////////////////////////////////////////////////////////////////////////
*								FollowCamera Class
////////////////////////////////////////////////////////////////////////////////*/
class FollowCamera {
public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	FollowCamera() = default;
	~FollowCamera() = default;

	void Init(const WorldTransform* target);

	void Update();

	void ImGui();

	//* getter *//

	Matrix4x4 GetViewProjectionMatrix() const { return viewProjectionMatrix_; }

	Vector3 GetTranslate() const { return transform_.translation; }

private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

	//===================================================================*/
	///* variables

	const WorldTransform* target_;

	WorldTransform transform_;

	Vector3 offset_;

	Matrix4x4 viewProjectionMatrix_;

};