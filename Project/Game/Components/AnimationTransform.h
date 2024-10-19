#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Game/Components/BaseTransform.h"

// c++
#include <optional>

/*////////////////////////////////////////////////////////////////////////////////
*						AnimationTransform Class
////////////////////////////////////////////////////////////////////////////////*/
class AnimationTransform :
	public BaseTransform {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	AnimationTransform() = default;
	~AnimationTransform() = default;

	void Init(const std::string& modelName, const std::string& animationName);
	void Update(const Matrix4x4& viewPro) override;

	// Setter
	void SetPlayAnimation(bool isPlayAnimation);

private:
	//===================================================================*/
	//							private Variables
	//===================================================================*/

	//* Animation *//
	AnimationData animationData_;
	std::optional<Skeleton> skeleton_;
	SkinCluster skinCluster_;
	ModelData modelData_;

	bool isPlayAnimation_;
	float animationtime_;

};