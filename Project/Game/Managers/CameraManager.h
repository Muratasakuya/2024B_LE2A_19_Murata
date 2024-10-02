#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Lib/Camera/Camera2D.h"
#include "Lib/Camera/Camera3D.h"

// c++
#include <memory>

// カメラタイプ
enum class CameraType {

	Perspective, // 3D
	Orthographic // 2D
};

/*////////////////////////////////////////////////////////////////////////////////
*									Model Class
////////////////////////////////////////////////////////////////////////////////*/
class CameraManager {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	CameraManager() = default;
	~CameraManager() = default;

	void Initialize();
	void Update();

	// Getter
	Matrix4x4 GetViewProjection(CameraType cameraType);

private:
	//===================================================================*/
	//							private Variables
	//===================================================================*/

	std::unique_ptr<Camera2D> camera2D_;
	std::unique_ptr<Camera3D> camera3D_;

};