#pragma once

// DXLib
#include "DXConstBuffer.h"

// 2DCamera
#include "MainCamera2D.h"

/*////////////////////////////////////////////////////////////////////////////////
								MatrixObject Class
////////////////////////////////////////////////////////////////////////////////*/
class MatrixObject :
	public DXConstBuffer<SpriteTransformationMatrix> {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	// default
	MatrixObject() = default;
	~MatrixObject() = default;

	// Main
	void Initialize();
	void Update(MainCamera2D* camera2D, const Transform2D& transform2D);

private:
	//===================================================================*/
	//							private Variable
	//===================================================================*/

	SpriteTransformationMatrix matrix_;

};