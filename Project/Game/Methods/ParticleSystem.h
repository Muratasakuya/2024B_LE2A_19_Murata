#pragma once
//===================================================================*/
//								include
//===================================================================*/
#include "Game/Components/VertexObject.h"
#include "Game/Components/IndexObject.h"
#include "Game/Components/ParticleBuffer.h"
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
	///* enum class

	enum class ParticleType {

		Dispersion,
		Chase
	};

	//===================================================================*/
	///* structs

	//* ParticleModel *//
	struct ParticleModel {

		ModelData data;
		std::vector<VertexObject<VertexData3D>> vertices;
		std::vector<IndexObject> indices;
	};

	//* ParticleGroup *//
	struct ParticleGroup {

		ParticleModel model;
		std::list<ParticleData> particles;
		uint32_t srvIndex;
		uint32_t numInstance;
		uint32_t instancingSrvIndex;
		ParticleBuffer particleBuffer;
		ParticleType type_;
		EasingType easingType_;
	};

public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	ParticleSystem() = default;
	~ParticleSystem() = default;

	void Update();

	void Draw(const std::string& name, BlendMode blendMode = BlendMode::kBlendModeNormal);

	//** CreateParticleMethods **//

	//* Dispersion
	void CreateDispersionParticle(
		const std::string& modelName,
		const std::string& name, const Vector3& centerPos,uint32_t emitCount,
		std::optional<float> speed = std::nullopt,
		std::optional<float> lifeTime = std::nullopt,
		const std::optional<Vector4>& color = std::nullopt);
	void EmitDispersionParticle(
		const std::string& name, const Vector3& centerPos, uint32_t emitCount,
		std::optional<float> speed = std::nullopt,
		std::optional<float> lifeTime = std::nullopt,
		const std::optional<Vector4>& color = std::nullopt);

	//* Chase
	void CreateChaseParticle(const std::string& modelName,
		const std::string& name, const Vector3& currentPos, const Vector3& prePos, uint32_t emitCount,
		std::optional<float> speed = std::nullopt,
		std::optional<float> lifeTime = std::nullopt,
		const std::optional<Vector4>& color = std::nullopt);
	void EmitChaseParticle(
		const std::string& name, const Vector3& currentPos, const Vector3& prePos, uint32_t emitCount,
		std::optional<float> speed = std::nullopt,
		std::optional<float> lifeTime = std::nullopt,
		const std::optional<Vector4>& color = std::nullopt);


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

};