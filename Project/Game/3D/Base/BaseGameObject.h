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
	virtual void DerivedImGui() {};

	// Setter
	//* engine
	void SetName(const std::string& name = "object");

	//* material
	void SetLightingEnable(bool enable);
	void SetUVScale(const Vector3& scale);


	// Getter
	std::string GetName() const;
	Vector3 GetWorldPos() const;
	Vector3 GetScale() const;

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

	std::string name_;

	uintptr_t ptrAddress_;

};