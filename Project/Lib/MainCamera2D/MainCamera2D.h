#pragma once

// dxLib
#include "CBufferStructure.h"

/*////////////////////////////////////////////////////////////////////////////////
								MainCamera2D Class
////////////////////////////////////////////////////////////////////////////////*/
class MainCamera2D {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	// default
	MainCamera2D() = default;
	~MainCamera2D() = default;

	// Main
	void Initialize();
	void Update();
	void ImGui();

	//===================================================================*/
	// getterMethods

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
