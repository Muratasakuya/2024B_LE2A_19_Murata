#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Engine/MyDirectXClass/DXConstBuffer.h"

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
	void Init();
	void Update(const Transform2D& transform2D);

private:
	//===================================================================*/
	//							private Variable
	//===================================================================*/

	SpriteTransformationMatrix matrix_;

};