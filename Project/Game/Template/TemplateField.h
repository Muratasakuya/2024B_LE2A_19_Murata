#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Game/3D/Base/BaseGameObject.h"

/*////////////////////////////////////////////////////////////////////////////////
*								TitleScene Class
////////////////////////////////////////////////////////////////////////////////*/
class TemplateField
	:public BaseGameObject {
public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	TemplateField() = default;
	~TemplateField() = default;

	void Init();

	void Update();

	void Draw();

	void DerivedImGui() override;

private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

	//===================================================================*/
	///* variables

	const std::string modelName_ = "originalField";

	Vector3 uvScale_;

	//===================================================================*/
	///* functions

	void CreateModel(int division);

	//* Json *//
	void SaveJson();
	void ApplyJson();

};