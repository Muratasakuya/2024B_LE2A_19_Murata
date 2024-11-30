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
#include <format>

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

	//* normal *//

	virtual void Init(const std::string& modelName);
	virtual void Update();
	virtual void Draw(BlendMode blendMode = kBlendModeNormal);

	//* animation *//

	virtual void Init(const std::string& modelName, const std::string& animationName);
	virtual void UpdateAnimation();
	virtual void DrawAnimation(BlendMode blendMode = kBlendModeNormal);

	//* imgui *//

	virtual void ImGui();
	virtual void DerivedImGui() {};

	//* utility *//

	virtual void ApplyJsonForColor();
	virtual void SaveJsonForColor();

	virtual void ApplyJsonForTransform(BaseTransform& transform);
	virtual void SaveJsonForTransform(const BaseTransform& transform);

	template <typename Derived>
	void AssignIndex();

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

	//===================================================================*/
	///* variables

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

	int index_;

	template <typename Derived>
	static int GenerateIndex();

};

template<typename Derived>
inline void BaseGameObject::AssignIndex() {

	index_ = GenerateIndex<Derived>();
}

template<typename Derived>
inline int BaseGameObject::GenerateIndex() {

	static int counter = 0;
	return counter++;
}