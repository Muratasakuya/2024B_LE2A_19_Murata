#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Game/3D/Model.h"
#include "Game/3D/AnimationModel.h"
#include "Lib/Math/Math.h"

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
	virtual void Init(const std::string& modelName, const std::string& animationName);

	virtual void Update();
	virtual void UpdateAnimation();

	virtual void Draw(BlendMode blendMode = kBlendModeNormal);
	virtual void DrawAnimation(BlendMode blendMode = kBlendModeNormal);

	virtual void ImGui();
	virtual void DerivedImGui() {};

	//* json *//

	virtual void ApplyJsonForTransform(BaseTransform& transform);
	virtual void SaveJsonForTransform(const BaseTransform& transform);

	//* setter *//

	void SetName(const std::string& name);
	void SetLightingEnable(bool enable);
	void SetAnimation(const std::string& animationName, bool play);
	void SetUVTransform(
		const std::optional<Vector3>& scale = std::nullopt,
		const std::optional<Vector3>& rotate = std::nullopt,
		const std::optional<Vector3>& translate = std::nullopt);
	void SetWorldTransform(const WorldTransform& transform);

	//* getter *//

	std::string GetName() const;
	Vector3 GetWorldPos() const;
	const WorldTransform& GetWorldTransform() const;

protected:
	//===================================================================*/
	//							protected Methods
	//===================================================================*/

	std::unique_ptr<Model> model_;
	std::unique_ptr<AnimationModel> animationModel_;

	WorldTransform transform_;
	AnimationTransform animationTransform_;

	Vector4 color_;

private:
	//===================================================================*/
	//							  private Methods
	//===================================================================*/

	std::vector<MaterialObject3D> materials_;

	std::string name_ = "object";

	bool isAnimationModel_;

};