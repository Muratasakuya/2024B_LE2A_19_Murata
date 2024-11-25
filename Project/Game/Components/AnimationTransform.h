#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Game/Components/BaseTransform.h"

// c++
#include <utility>
#include <optional>
#include <unordered_map>

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
	void Update() override;

	// Setter
	void SetPlayAnimation(bool isPlayAnimation, const std::string& animationName);
	void SetNewAnimationData(const std::string& animationName);

private:
	//===================================================================*/
	//							private Variables
	//===================================================================*/

	//* Animation *//
	std::unordered_map<std::string, AnimationData> animationData_;
	std::unordered_map<std::string, std::optional<Skeleton>> skeleton_;
	std::unordered_map<std::string, SkinCluster> skinCluster_;
	ModelData modelData_;

	std::pair<bool, std::string> animationController_;
	float animationTime_;

};