#pragma once

//===================================================================*/
//	include
//===================================================================*/
#include "Game/3D/Model.h"
#include "Game/3D/AnimationModel.h"

/*////////////////////////////////////////////////////////////////////////////////
*	IBaseGameObject Class
////////////////////////////////////////////////////////////////////////////////*/
class IBaseGameObject {
public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	IBaseGameObject() = default;
	virtual ~IBaseGameObject();

	virtual void Draw(BlendMode blendMode = kBlendModeNormal) = 0;

	//* imgui *//

	virtual void ImGui();
	virtual void TransformImGui() = 0;
	virtual void DerivedImGui() = 0;

	//* utility *//

	virtual void ApplyJsonForColor();
	virtual void SaveJsonForColor();

	virtual void ApplyJsonForTransform(BaseTransform& transform);
	virtual void SaveJsonForTransform(const BaseTransform& transform);

	//* Json *//

	virtual void ApplyJson() = 0;
	virtual void SaveJson() = 0;

	//* setter *//

	void SetMeshRenderer(const std::string& name, uint32_t index = 0);

	void SetUVTransform(
		const std::optional<Vector3>& scale = std::nullopt,
		const std::optional<Vector3>& rotate = std::nullopt,
		const std::optional<Vector3>& translate = std::nullopt);

	void SetLightingEnable(bool enable);

	//* getter *//

	std::string GetName() const { return name_; };

protected:
	//===================================================================*/
	//							protected Methods
	//===================================================================*/

	//===================================================================*/
	///* variables

	Vector4 color_;

	std::optional<std::string> parentFolderName_;

	std::vector<MaterialObject3D> materials_;

private:
	//===================================================================*/
	//							  private Methods
	//===================================================================*/

	std::string name_ = "object";

};