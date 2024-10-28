#pragma once
//===================================================================*/
//								include
//===================================================================*/
#include "Game/Components/VertexObject.h"
#include "Game/Components/IndexObject.h"
#include "Game/Components/ParticleBuffer.h"
#include "Engine/MyDirectXClass/Pipeline/PipelineStateStructure.h"
#include "Lib/Structure.h"

// c++
#include <unordered_map>

/*////////////////////////////////////////////////////////////////////////////////
*							ParticleSystem Class
////////////////////////////////////////////////////////////////////////////////*/
class ParticleSystem {
private:
	//===================================================================*/
	//							private Struct
	//===================================================================*/

	//* ParticleGroup *//
	struct ParticleGroup {

		ParticleMaterialData material;
		std::list<ParticleData> particles;
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

	void Init();

	void Update();

	void Draw(const std::string& name, const std::string& textureName, BlendMode blendMode = BlendMode::kBlendModeNormal);

	void Emit(const std::string name, const Vector3& pos, uint32_t count);

	void CreateParticleGroup(const Vector3& pos, const std::string name, const std::string& textureName, uint32_t count);

private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

	//===================================================================*/
	///* variables

	const uint32_t instanceMaxCount_ = 128; //* Instancing

	//* IA *//
	static const size_t kParticleModelVertexNum = 4;
	static const size_t kParticleModelIndexNum = 6;

	std::array<VertexData3D, kParticleModelVertexNum> vertexDataArr_;
	VertexObject<VertexData3D> vertex_;
	IndexObject index_;

	std::unordered_map<std::string, ParticleGroup> particleGroups_;

	//===================================================================*/
	///* function

	void CreateVertexData();

};