#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Engine/MyDirectXClass/DXConstBuffer.h"
#include "Lib/Camera/MainCamera3D.h"

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

	void Initialize();
	void Update(MainCamera3D* camera3D);

	// Setter
	void SetAnimationData(const std::string& modelName, const std::string& animationName);
	void SetPlayAnimation(bool isPlayAnimation);

public:
	//===================================================================*/
	//							public Variables
	//===================================================================*/

	Vector3 scale;
	Vector3 rotation;
	Vector3 translation;

	Quaternion quaternionRotation;

	TransformationMatrix matrix;

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

private:
	//===================================================================*/
	//							private Function
	//===================================================================*/

	

};