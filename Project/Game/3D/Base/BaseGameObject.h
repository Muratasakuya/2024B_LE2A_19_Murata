#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Game/3D/Model.h"
#include "Game/3D/AnimationModel.h"

// c++
#include <vector>
#include <memory>
#include <string>

/*////////////////////////////////////////////////////////////////////////////////
*						BaseGameObject Class
////////////////////////////////////////////////////////////////////////////////*/
class BaseGameObject {
public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	BaseGameObject() = default;
	virtual ~BaseGameObject() = default;

	virtual void Init(const std::string& modelName);

	virtual void Update(const Matrix4x4& viewPro);

	virtual void Draw(BlendMode blendMode = kBlendModeNormal);

	virtual void ImGui();

	// Setter
	void SetName(const std::string& name);
	void SetLightingEnable(bool enable);

	// Getter
	std::string GetName() const;

protected:
	//===================================================================*/
	//							protected Methods
	//===================================================================*/

	std::unique_ptr<Model> model_;

	WorldTransform transform_;
	Vector4 color_;

private:
	//===================================================================*/
	//							  private Methods
	//===================================================================*/

	std::vector<MaterialObject3D> materials_;

	std::string name_ = "object";

};