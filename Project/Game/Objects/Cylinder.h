#pragma once

#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Game/3D/Base/BaseGameObject.h"

// c++
#include <memory>
#include <vector>
#include <string>

/*////////////////////////////////////////////////////////////////////////////////
*								Ground Class
////////////////////////////////////////////////////////////////////////////////*/
class Cylinder :
	public BaseGameObject {
public:
	//===================================================================*/
	//							public Function
	//===================================================================*/

	Cylinder() = default;
	~Cylinder() = default;

	void Init(uint32_t index, const Vector3& pos, const Vector3& scale);

	void Update();

	void Draw();

private:
	//===================================================================*/
	//							private variable
	//===================================================================*/



};

/*////////////////////////////////////////////////////////////////////////////////
*								Ground Class
////////////////////////////////////////////////////////////////////////////////*/
class CylinderCollection {
public:
	//===================================================================*/
	//							public Function
	//===================================================================*/

	CylinderCollection() = default;
	~CylinderCollection() = default;

	void Init();

	void Update();

	void Draw();

	void ImGui();

	void ApplyJson();
	void SaveJson();

public:
	//===================================================================*/
	//							private variable
	//===================================================================*/

	std::vector<std::unique_ptr<Cylinder>> cylinders_;

	uint32_t index_;

};