#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Game/Components/CameraObject.h"
#include "Lib/Camera/Camera2D.h"
#include "Lib/Camera/Camera3D.h"
#include "Lib/Camera/DebugCamera.h"

// c++
#include <memory>

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

	void Init();
	void Update();

	void ImGui(bool debugCameraEnable);

	// Getter
	Camera2D* GetCamera2D() const;
	Camera3D* GetCamera3D() const;
	DebugCamera* GetDebugCamera() const;

private:
	//===================================================================*/
	//							private Variables
	//===================================================================*/

	std::unique_ptr<Camera2D> camera2D_;
	std::unique_ptr<Camera3D> camera3D_;

	std::unique_ptr<DebugCamera> debugCamera_;

};