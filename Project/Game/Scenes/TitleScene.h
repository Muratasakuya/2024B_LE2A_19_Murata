#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Game/Scenes/Methods/IScene.h"

//* object
#include "Game/Template/TestParticle.h"
#include "Game/Template/TestGameObject.h"
#include "Game/Template/TemplateField.h"

/*////////////////////////////////////////////////////////////////////////////////
*								TitleScene Class
////////////////////////////////////////////////////////////////////////////////*/
class TitleScene :
	public IScene {
public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	TitleScene();
	~TitleScene();

	void Run() override;

	void Init() override;

	void Update() override;

	void Draw();

	void Cleanup() override;

private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

	const std::string& baseModelDirectory_ = "./Resources/Obj";

	std::unique_ptr<TestGameObject> objects_;

	std::unique_ptr<TemplateField> field_;

	std::unique_ptr<TestParticle> particle_;

private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

	void Load();

};