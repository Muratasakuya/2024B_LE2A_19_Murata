#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Engine/MyDirectXClass/DXConstBuffer.h"
#include "Lib/Camera/Camera2D.h"

/*////////////////////////////////////////////////////////////////////////////////
								MatrixObject Class
////////////////////////////////////////////////////////////////////////////////*/
class MatrixObject :
	public DXConstBuffer<SpriteTransformationMatrix> {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	MatrixObject() = default;
	~MatrixObject() = default;

	// Main
	void Initialize();
	void Update(Camera2D* camera2D, const Transform2D& transform2D);

private:
	//===================================================================*/
	//							private Variable
	//===================================================================*/

	SpriteTransformationMatrix matrix_;

};