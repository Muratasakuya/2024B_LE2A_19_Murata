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
	virtual void Update() = 0;

	virtual void SetCommand(ID3D12GraphicsCommandList* commandList);

	// Getter
	virtual Vector3 GetWorldPos() const;

public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	Vector3 scale;
	Vector3 translation;
	Quaternion rotation;

	Vector3 eulerRotate;

	TransformationMatrix matrix;

private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

	const UINT rootParameterIndex_ = 1;

};