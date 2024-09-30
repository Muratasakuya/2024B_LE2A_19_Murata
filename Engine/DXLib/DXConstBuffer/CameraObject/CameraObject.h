#pragma once

// DXLib
#include "DXConstBuffer.h"

// 3DCamera
#include "MainCamera3D.h"

/*////////////////////////////////////////////////////////////////////////////////
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

	void Initialize();
	void Update(const Vector3& worldPos);

private:
	//===================================================================*/
	//							private Variables
	//===================================================================*/

	CameraForGPU camera;

};