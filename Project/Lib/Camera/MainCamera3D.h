#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Lib/Structure.h"

// c++
#include <numbers>

/*////////////////////////////////////////////////////////////////////////////////
*								Camera3D Class
////////////////////////////////////////////////////////////////////////////////*/
class MainCamera3D {
public:
	//===================================================================*/
	//							public Function
	//===================================================================*/

	// default
	MainCamera3D() = default;
	~MainCamera3D() = default;

	// Main
	void Initialize();
	void Update();
	void ImGui();

	// Getter
	Vector3 GetWorldPos() const;
	Matrix4x4 GetCameraMatrix() const;
	Matrix4x4 GetViewMatrix() const;
	Matrix4x4 GetProjectionMatrix() const;
	Matrix4x4 GetViewProjectionMatrix() const;
	Matrix4x4 GetProjectionInverseMatrix() const;

public:
	//===================================================================*/
	//							private variable
	//===================================================================*/

	Matrix4x4 cameraMatrix_;
	Matrix4x4 viewMatrix_;
	Matrix4x4 projectionMatrix_;

	Matrix4x4 viewProjectionMatrix_;
	Matrix4x4 projectionInverseMatrix_;

	Transform3D transform_;
};
