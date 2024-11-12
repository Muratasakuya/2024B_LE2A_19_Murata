#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Engine/MyDirectXClass/Pipeline/PipelineStateStructure.h"
#include "Game/Components/CameraObject.h"
#include "Game/Components/WorldTransform.h"
#include "Game/Editor/RailEditor.h"
#include "Lib/Camera/Camera3D.h"
#include "Lib/Structure.h"
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

	void ImGui();

	void CameraInit();
	void GuiCameraUpdate();

	// Getter
	Vector3 GetWorldPos() const;
	Vector3 GetFoward() const;
	Matrix4x4 GetViewProjectionMatrix() const;
	WorldTransform& GetTransform();
	bool IsStart() const;
	float GetTimeT() const { return t_; }
	float GetReparamT() const { return reparamT_; }

public:
	//===================================================================*/
	//							private variable
	//===================================================================*/

	RailEditor* railEditor_ = nullptr;

	WorldTransform transform_;
	std::unique_ptr<Camera3D> camera_;

	Vector3 forward_;

	CameraObject cameraBuffer_;
	ViewProjectionBuffer viewProBuffer_;

	float t_;

	bool isStart_; //* スタート
	bool guiUpdate_;

	float deltaRate_; //* レールの速度管理
	float targetDeltaRate_ = 0.78f;

	float timer_ = 0.0f;
	const float durationtime_ = 180.0f;
	float reparamT_;

};