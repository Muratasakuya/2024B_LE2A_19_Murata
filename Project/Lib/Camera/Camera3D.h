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
class Camera3D {
public:
	//===================================================================*/
	//							public Function
	//===================================================================*/

	Camera3D() = default;
	~Camera3D() = default;

	// Main
	void Init();
	void Update();
	void ImGui();

	// Setter
	void SetViewMatrix(const Matrix4x4& viewMatrix);

	// Getter
	Vector3 GetWorldPos() const;
	Matrix4x4 GetCameraMatrix() const;
	Matrix4x4 GetViewMatrix() const;
	Matrix4x4 GetProjectionMatrix() const;
	Matrix4x4 GetViewProjectionMatrix() const;
	Matrix4x4 GetProjectionInverseMatrix() const;

private:
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
