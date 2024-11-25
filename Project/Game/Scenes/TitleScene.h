#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Game/Scenes/Methods/IScene.h"

//* object
#include "Game/Objects/TestParticle.h"
#include "Game/Objects/TestGameObject.h"

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

	std::unique_ptr<TestGameObject> object_;

	std::unique_ptr<TestParticle> particle_;

private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

	void Load();

};