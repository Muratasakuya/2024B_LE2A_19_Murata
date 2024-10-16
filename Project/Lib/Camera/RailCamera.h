#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Game/Components/WorldTransform.h"
#include "Game/Editor/RailEditor.h"
#include "Lib/Structure.h"
#include "Lib/Camera/Camera3D.h"

// c++
#include <memory>
#include <numbers>

/*////////////////////////////////////////////////////////////////////////////////
*								RailCamera Class
////////////////////////////////////////////////////////////////////////////////*/
class RailCamera {
public:
	//===================================================================*/
	//							public Function
	//===================================================================*/

	RailCamera() = default;
	~RailCamera() = default;

	void Init(RailEditor* railEditor, const Vector3& worldPos);
	void Update();

	// Getter
	Vector3 GetWorldPos() const;
	Matrix4x4 GetViewProjectionMatrix() const;

public:
	//===================================================================*/
	//							private variable
	//===================================================================*/

	RailEditor* railEditor_ = nullptr;

	WorldTransform worldTransform_;
	std::unique_ptr<Camera3D> camera_;

	float eyeT_;
	float targetT_;

};