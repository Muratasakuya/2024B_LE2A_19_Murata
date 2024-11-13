#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Engine/MyDirectXClass/Pipeline/PipelineStateStructure.h"
#include "Game/Components/WorldTransform.h"
#include "Game/Components/CameraObject.h"
#include "Lib/Structure.h"

// c++
#include <numbers>

/*////////////////////////////////////////////////////////////////////////////////
*								Camera3D Class
////////////////////////////////////////////////////////////////////////////////*/
class Camera3D {
private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

	struct CameraData {

		Matrix4x4 matrix;
		Matrix4x4 viewMatrix;
		Matrix4x4 projectionMatrix;

		Matrix4x4 viewProjectionMatrix;
		Matrix4x4 projectionInverseMatrix;

		Transform3D transform;

		CameraData& operator=(const CameraData& other) {
			if (this != &other) {
				this->matrix = other.matrix;
				this->viewMatrix = other.viewMatrix;
				this->projectionMatrix = other.projectionMatrix;
				this->viewProjectionMatrix = other.viewProjectionMatrix;
				this->projectionInverseMatrix = other.projectionInverseMatrix;
				this->transform.translate = other.transform.translate;
				this->transform.rotate = other.transform.rotate;
			}
			return *this;
		}
	};

public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	Camera3D() = default;
	~Camera3D() = default;

	// Main
	void Init();
	void Update();
	void ImGui();

	void Reset();

	void SetCommand(ID3D12GraphicsCommandList* commandList, const PipelineType& pipelineType);
	void ViewProSetCommand(ID3D12GraphicsCommandList* commandList);

	// Setter
	void SetViewMatrix(const Matrix4x4& viewMatrix);
	void SetProjectionMatrix(const Matrix4x4& projectionMatrix);
	void SetCamera(const Matrix4x4& viewProMatrix, const Vector3& pos);
	void SetTranslate(const Vector3& translate);
	void SetRotate(const Vector3& rotate);

	// Getter
	Vector3 GetWorldPos() const;
	Vector3 GetRotate() const;
	Matrix4x4 GetCameraMatrix() const;
	Matrix4x4 GetViewMatrix() const;
	Matrix4x4 GetProjectionMatrix() const;
	Matrix4x4 GetViewProjectionMatrix() const;
	Matrix4x4 GetProjectionInverseMatrix() const;
	CameraObject GetCameraBuffer() const;
	ViewProjectionBuffer GetViewProBuffer() const;

private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

	CameraData data_;
	CameraData resetData_;

	CameraObject cameraBuffer_;
	ViewProjectionBuffer viewProBuffer_;

};
