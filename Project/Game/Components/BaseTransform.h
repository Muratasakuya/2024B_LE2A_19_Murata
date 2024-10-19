#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Engine/MyDirectXClass/DXConstBuffer.h"

/*////////////////////////////////////////////////////////////////////////////////
*								BaseTransform Class
////////////////////////////////////////////////////////////////////////////////*/
class BaseTransform :
	public DXConstBuffer<TransformationMatrix> {
public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	BaseTransform() = default;
	virtual ~BaseTransform() = default;

	virtual void Init();
	virtual void Update(const Matrix4x4& viewPro) = 0;

	virtual void SetCommand();

public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	Vector3 scale;
	Vector3 rotation;
	Vector3 translation;

	TransformationMatrix matrix;

private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

	const UINT rootParameterIndex_ = 1;

};