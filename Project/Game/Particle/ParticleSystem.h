#pragma once
//===================================================================*/
//								include
//===================================================================*/
#include "Game/Components/ParticleBuffer.h"
#include "Game/Components/InputAssembler.h"
#include "Game/Particle/ParticleBehavior.h"
#include "Game/Particle/ParticleParameter.h"
#include "Engine/MyDirectXClass/Pipeline/PipelineStateStructure.h"
#include "Lib/Structure.h"
#include "Lib/Adapter/Easing.h"

// c++
#include <unordered_map>
#include <optional>

/*////////////////////////////////////////////////////////////////////////////////
*							ParticleSystem Class
////////////////////////////////////////////////////////////////////////////////*/
class ParticleSystem {
private:
	//===================================================================*/
	//								private
	//===================================================================*/
	
	//===================================================================*/
	///* structs

	//* ParticleModel *//
	struct ParticleModel {

		ModelData data;
		InputAssembler inputAssembler;
	};

	//* ParticleGroup *//
	struct ParticleGroup {

		ParticleModel model;
		std::list<ParticleData> particles;
		std::unique_ptr<ParticleBehavior> behavior;
		uint32_t srvIndex;
		uint32_t numInstance;
		uint32_t instancingSrvIndex;
		ParticleBuffer particleBuffer;
	};

public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	ParticleSystem() = default;
	~ParticleSystem() = default;

	void Update();

	void Draw(const std::string& name, BlendMode blendMode = BlendMode::kBlendModeNormal);

	void CreateParticle(
		const std::string& modelName, const std::string& name,
		ParticleType particleType, ParticleParameter& parameter);

	void EmitParticle(
		const std::string& name, ParticleType particleType, ParticleParameter& parameter);

private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

	//===================================================================*/
	///* variables

	const uint32_t instanceMaxCount_ = 128; //* Instancing

	std::unordered_map<std::string, ParticleGroup> particleGroups_;

	//===================================================================*/
	///* function

	void CreateVertexData(const std::string& name);
	Matrix4x4 CalBillboardMatrix() const;

};