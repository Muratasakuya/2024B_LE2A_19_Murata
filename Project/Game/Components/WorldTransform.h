#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Engine/MyDirectXClass/DXConstBuffer.h"

/*////////////////////////////////////////////////////////////////////////////////
*								WorldTransform Class
////////////////////////////////////////////////////////////////////////////////*/
class WorldTransform :
	public DXConstBuffer<TransformationMatrix> {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	WorldTransform() = default;
	~WorldTransform() = default;

	void Init();
	void Update();

	// Setter
	void SetAnimationData(const std::string& modelName, const std::string& animationName);
	void SetPlayAnimation(bool isPlayAnimation);

	// Getter
	UINT GetRootParameterIndex() const;

public:
	//===================================================================*/
	//							public Variables
	//===================================================================*/

	Vector3 scale;
	Vector3 rotation;
	Vector3 translation;

	Quaternion quaternionRotation;

	TransformationMatrix matrix;

	// 親
	const WorldTransform* parent_ = nullptr;

private:
	//===================================================================*/
	//							private Variables
	//===================================================================*/

	// アニメーション
	AnimationData animationData_;
	std::optional<Skeleton> skeleton_;
	SkinCluster skinCluster_;
	ModelData modelData_;
	bool isPlayAnimation_;
	float animationtime_;

};