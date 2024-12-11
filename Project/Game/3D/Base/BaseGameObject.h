#pragma once

//===================================================================*/
//	include
//===================================================================*/
#include "Game/3D/Base/IBaseGameObject.h"
#include "Game/Components/WorldTransform.h"

/*////////////////////////////////////////////////////////////////////////////////
*	BaseGameObject Class
////////////////////////////////////////////////////////////////////////////////*/
class BaseGameObject :
	public IBaseGameObject {
public:
	//===================================================================*/
	//	public Methods
	//===================================================================*/

	BaseGameObject() = default;
	virtual ~BaseGameObject() = default;

	virtual void Init(const std::string& modelName);

	virtual void Update();

	virtual void Draw(BlendMode blendMode = kBlendModeNormal) override;

	//* imgui *//

	void TransformImGui() override;
	virtual void DerivedImGui() override {}

	//* utility *//

	virtual void ApplyJson() override {};
	virtual void SaveJson() override {};

	//* setter *//

	void SetWorldTransform(const WorldTransform& transform);

	//* getter *//

	const WorldTransform& GetWorldTransform() const { return transform_; };

	Vector3 GetWorldPos() const { return transform_.GetWorldPos(); };

protected:
	//===================================================================*/
	//	protected Methods
	//===================================================================*/

	//===================================================================*/
	///* variables

	std::unique_ptr<Model> model_;

	WorldTransform transform_;

};