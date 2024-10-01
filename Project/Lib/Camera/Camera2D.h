#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Lib/Structure.h"

/*////////////////////////////////////////////////////////////////////////////////
									Camera2D Class
////////////////////////////////////////////////////////////////////////////////*/
class Camera2D {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	Camera2D() = default;
	~Camera2D() = default;

	// Main
	void Initialize();
	void Update();
	void ImGui();

	// Getter
	Matrix4x4 GetViewMatrix() const;
	Matrix4x4 GetOrthoMatrix() const;
	Matrix4x4 GetViewProjectionMatrix() const;

public:
	//===================================================================*/
	//							private Variables
	//===================================================================*/

	Matrix4x4 cameraMatrix_;
	Matrix4x4 viewMatrix_;
	Matrix4x4 orthoMatrix_;

	Matrix4x4 viewProjectionMatrix_;

	Transform3D transform_;
};
