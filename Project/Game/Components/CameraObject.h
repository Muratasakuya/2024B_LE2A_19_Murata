#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Engine/MyDirectXClass/DXConstBuffer.h"

/*/////////////////////////////////////////////////////////////////////////////3///
*								CameraObject Class
////////////////////////////////////////////////////////////////////////////////*/
class CameraObject :
	public DXConstBuffer<CameraForGPU> {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	CameraObject() = default;
	~CameraObject() = default;

	void Init();
	void Update(const Vector3& worldPos);

private:
	//===================================================================*/
	//							private Variables
	//===================================================================*/

	CameraForGPU camera;

};

/*/////////////////////////////////////////////////////////////////////////////3///
*						ViewProjectionBuffer Class
////////////////////////////////////////////////////////////////////////////////*/
class ViewProjectionBuffer :
	public DXConstBuffer<Matrix4x4> {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	ViewProjectionBuffer() = default;
	~ViewProjectionBuffer() = default;

	void Init();
	void Update(Matrix4x4 viewPro);

private:
	//===================================================================*/
	//							private Variables
	//===================================================================*/

	Matrix4x4 viewProjection;

};