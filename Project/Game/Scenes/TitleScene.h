#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Game/Scenes/Methods/IScene.h"

//* object
#include "Game/Objects/Player.h"
#include "Game/Objects/StaticMeshModel.h"
#include "Game/Template/TemplateField.h"

//* particle
#include "Game/Particles/FieldParticle.h"
#include "Game/Particles/InjectionParticle.h"
#include "Game/Particles/DispersionParticle.h"

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

	std::unique_ptr<Player> player_;

	std::unique_ptr<StaticMeshModel> staticMesh_;

	std::unique_ptr<TemplateField> field_;

	std::unique_ptr<FieldParticle> fieldParticle_;

	std::unique_ptr<InjectionParticle> injectionParticle_;

	std::unique_ptr<DispersionParticle> dispersionParticle;

private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

	void Load();

};