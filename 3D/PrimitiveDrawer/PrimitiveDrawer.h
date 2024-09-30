#pragma once

// MainObject
#include "VertexObject.h"

/*////////////////////////////////////////////////////////////////////////////////
*								PrimitiveDrawer Class
////////////////////////////////////////////////////////////////////////////////*/
class PrimitiveDrawer {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	PrimitiveDrawer() = default;
	~PrimitiveDrawer() = default;

	void Initialize();
	void Update();
	void DrawLine();

private:
	//===================================================================*/
	//							private Variables
	//===================================================================*/

	VertexObject<VertexData3D> vertex_;

};