#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Lib/Structure.h"

/*////////////////////////////////////////////////////////////////////////////////
*							DebugCamera Class
////////////////////////////////////////////////////////////////////////////////*/
class DebugCamera {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	DebugCamera();
	~DebugCamera() = default;

	void Update(const Vector3& pos, const Vector3& rotate);

	void ImGui();

	// Setter
	void SetEnable(bool enable);

	// Getter
	Vector3 GetTranslate() const;
	Vector3 GetRotate() const;
	bool Enable() const;

private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

	//===================================================================*/
	///* variables

	Vector3 translation_; //* 移動座標
	Vector3 rotation_;    //* 回転

	Matrix4x4 rotateMatrix_; //* 回転行列
	Matrix4x4 matrix_;       //* World行列

	bool enable_; //* ON:OFF 切り替え

	//* parameter *//

	float zoomRate_; //* マウスホイール移動感度

	//===================================================================*/
	///* functions

	void Move(); //* 移動

};