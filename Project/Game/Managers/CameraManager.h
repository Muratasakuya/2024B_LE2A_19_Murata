#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Game/Components/CameraObject.h"
#include "Lib/Camera/Camera2D.h"
#include "Lib/Camera/Camera3D.h"
#include "Lib/Camera/RailCamera.h"

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

	void Init();
	void Update();

	void ImGui();

	void SetUpRailCamera(RailEditor* railEditor, const Vector3& initPos);

	// Getter
	Camera2D* GetCamera2D() const;
	Camera3D* GetCamera3D() const;
	RailCamera* GetRailCamera() const;

private:
	//===================================================================*/
	//							private Variables
	//===================================================================*/

	std::unique_ptr<Camera2D> camera2D_;
	std::unique_ptr<Camera3D> camera3D_;

	std::unique_ptr<RailCamera> railCamera_;

};