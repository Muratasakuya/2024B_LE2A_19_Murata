#pragma once

//===================================================================*/
//	include
//===================================================================*/
#include "Game/3D/Base/IBaseGameObject.h"
#include "Game/Components/AnimationTransform.h"

/*////////////////////////////////////////////////////////////////////////////////
*	BaseAnimationObject Class
////////////////////////////////////////////////////////////////////////////////*/
class BaseAnimationObject :
	public IBaseGameObject {
public:
	//===================================================================*/
	//	public Methods
	//===================================================================*/

	BaseAnimationObject() = default;
	virtual ~BaseAnimationObject() = default;

	virtual void Init(const std::string& modelName, const std::string& animationName);

	virtual void Update();

	virtual void Draw(BlendMode blendMode = kBlendModeNormal) override;

	//* imgui *//

	void TransformImGui() override;
	virtual void DerivedImGui() override {}

	//* utility *//

	virtual void ApplyJson() override {};
	virtual void SaveJson() override {};

	//* setter *//

	void SetAnimation(const std::string& animationName, bool play);

	void SetWorldTransform(const AnimationTransform& transform);

	//* getter *//

	const AnimationTransform& GetWorldTransform() const { return transform_; };

	Vector3 GetWorldPos() const { return transform_.GetWorldPos(); };

protected:
	//===================================================================*/
	//	protected Methods
	//===================================================================*/

	//===================================================================*/
	///* variables

	std::unique_ptr<AnimationModel> model_;

	AnimationTransform transform_;

};