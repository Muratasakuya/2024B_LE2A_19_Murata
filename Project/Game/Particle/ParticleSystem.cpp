#include "ParticleSystem.h"

#include "Engine/Base/NewMoon.h"
#include "Engine/Base/NewMoonGame.h"
#include "Engine/Managers/SrvManager.h"
#include "Lib/Adapter/Random.h"

/*////////////////////////////////////////////////////////////////////////////////
*							ParticleSystem classMethods
////////////////////////////////////////////////////////////////////////////////*/

void ParticleSystem::CreateVertexData(const std::string& name) {

	size_t meshNum = particleGroups_[name].model.data.meshes.size();

	particleGroups_[name].model.vertices.resize(meshNum);
	particleGroups_[name].model.indices.resize(meshNum);

	for (uint32_t index = 0; index < meshNum; ++index) {

		particleGroups_[name].model.vertices[index].Init(static_cast<UINT>(particleGroups_[name].model.data.meshes[index].vertices.size()));
		particleGroups_[name].model.indices[index].Init(static_cast<UINT>(particleGroups_[name].model.data.meshes[index].indices.size()));

		particleGroups_[name].model.vertices[index].data.resize(particleGroups_[name].model.data.meshes[index].vertices.size());
		std::copy(
			particleGroups_[name].model.data.meshes[index].vertices.begin(),
			particleGroups_[name].model.data.meshes[index].vertices.end(),
			particleGroups_[name].model.vertices[index].data.begin());

		particleGroups_[name].model.indices[index].data.resize(particleGroups_[name].model.data.meshes[index].indices.size());
		std::copy(
			particleGroups_[name].model.data.meshes[index].indices.begin(),
			particleGroups_[name].model.data.meshes[index].indices.end(),
			particleGroups_[name].model.indices[index].data.begin());

		// 転送
		particleGroups_[name].model.vertices[index].Update();
		particleGroups_[name].model.indices[index].Update();
	}

}

Matrix4x4 ParticleSystem::CalBillboardMatrix() const {

	Matrix4x4 backToFrontMatrix = Matrix4x4::MakeYawMatrix(std::numbers::pi_v<float>);
	Matrix4x4 billboardMatrix =
		Matrix4x4::Multiply(backToFrontMatrix, NewMoonGame::GameCamera()->GetCamera3D()->GetCameraMatrix());
	billboardMatrix.m[3][0] = 0.0f;
	billboardMatrix.m[3][1] = 0.0f;
	billboardMatrix.m[3][2] = 0.0f;

	return billboardMatrix;
}

void ParticleSystem::Update() {

	Matrix4x4 billboardMatrix = CalBillboardMatrix();
	for (auto& [name, group] : particleGroups_) {

		auto& particles = group.particles;
		group.particleBuffer.properties.resize(particles.size());

		for (auto it = particles.begin(); it != particles.end();) {

			// 寿命が切れたら削除
			if (it->lifeTime <= it->currentTime) {
				it = particles.erase(it);
				continue;
			}

			group.behavior->Update(*it, billboardMatrix);

			int index = static_cast<uint32_t>(std::distance(particles.begin(), it));
			if (index < group.particleBuffer.properties.size()) {

				group.particleBuffer.properties[index].World = it->worldMatrix;
				group.particleBuffer.properties[index].WVP = it->wvpMatrix;
				group.particleBuffer.properties[index].color = it->color;
			}

			++it;
		}

		group.numInstance = static_cast<uint32_t>(particles.size());
		group.particleBuffer.Update();
	}

}

void ParticleSystem::Draw(const std::string& name, BlendMode blendMode) {

	auto commandList = NewMoon::GetCommandList();

	for (uint32_t index = 0; index < particleGroups_[name].model.data.meshes.size(); ++index) {

		NewMoon::SetGraphicsPipeline(commandList, pParticle, blendMode);
		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		commandList->IASetVertexBuffers(0, 1, &particleGroups_[name].model.vertices[index].GetVertexBuffer());
		commandList->IASetIndexBuffer(&particleGroups_[name].model.indices[index].GetIndexBuffer());
		particleGroups_[name].particleBuffer.SetCommand(commandList, particleGroups_[name].particleBuffer.GetRootParameterIndex());
		NewMoon::SetGraphicsRootDescriptorTable(commandList, 1, particleGroups_[name].model.data.meshes[index].material.textureName.value());
		commandList->SetGraphicsRootDescriptorTable(2, NewMoon::GetSrvManagerPtr()->GetGPUHandle(particleGroups_[name].instancingSrvIndex));
		commandList->DrawIndexedInstanced(static_cast<UINT>(particleGroups_[name].model.indices[index].data.size()), particleGroups_[name].numInstance, 0, 0, 0);
	}
}

void ParticleSystem::CreateParticle(
	const std::string& modelName, const std::string& name,
	ParticleBehaviorType behaiviorType, const ParticleParameter& parameter) {

	assert(particleGroups_.find(name) == particleGroups_.end() && "Particle group with this name already exists");

	particleGroups_[name].behavior = ParticleBehaviorFactory::CreateBehavior(behaiviorType);

	for (uint32_t index = 0; index < parameter.count; ++index) {

		ParticleData particle{};

		particleGroups_[name].behavior->Create(particle, parameter);
		particleGroups_[name].particles.push_back(particle);
	}

	//!! alreadyLoadModel !!//
	particleGroups_[name].model.data = NewMoonGame::GetModelMangager()->GetModelData(modelName);
	CreateVertexData(name);

	//* CreateStructureBuffer *//
	particleGroups_[name].srvIndex = NewMoon::GetSrvManagerPtr()->Allocate();
	particleGroups_[name].numInstance = instanceMaxCount_;
	particleGroups_[name].particleBuffer.Init(particleGroups_[name].numInstance);
	particleGroups_[name].instancingSrvIndex = NewMoon::GetSrvManagerPtr()->Allocate();
	NewMoon::GetSrvManagerPtr()->CreateSRVForStructureBuffer(
		particleGroups_[name].instancingSrvIndex, particleGroups_[name].particleBuffer.GetResource(),
		particleGroups_[name].numInstance, sizeof(ParticleForGPU));

}

void ParticleSystem::PresetCreateParticle(
	const std::string& modelName, const std::string& name,
	ParticleBehaviorType behaiviorType, const ParticleParameter& parameter) {

	assert(particleGroups_.find(name) == particleGroups_.end() && "Particle group with this name already exists");

	particleGroups_[name].behavior = ParticleBehaviorFactory::CreateBehavior(behaiviorType);

	std::vector<ParticleData> particles{};

	particleGroups_[name].behavior->PresetCreate(particles, parameter);
	particleGroups_[name].particles.resize(particles.size());
	std::copy(particles.begin(), particles.end(), particleGroups_[name].particles.begin());

	//!! alreadyLoadModel !!//
	particleGroups_[name].model.data = NewMoonGame::GetModelMangager()->GetModelData(modelName);
	CreateVertexData(name);

	//* CreateStructureBuffer *//
	particleGroups_[name].srvIndex = NewMoon::GetSrvManagerPtr()->Allocate();
	particleGroups_[name].numInstance = instanceMaxCount_;
	particleGroups_[name].particleBuffer.Init(particleGroups_[name].numInstance);
	particleGroups_[name].instancingSrvIndex = NewMoon::GetSrvManagerPtr()->Allocate();
	NewMoon::GetSrvManagerPtr()->CreateSRVForStructureBuffer(
		particleGroups_[name].instancingSrvIndex, particleGroups_[name].particleBuffer.GetResource(),
		particleGroups_[name].numInstance, sizeof(ParticleForGPU));
}

void ParticleSystem::EmitParticle(
	const std::string& name, ParticleBehaviorType behaiviorType, const ParticleParameter& parameter) {

	// 既存のパーティクルグループを検索
	auto it = particleGroups_.find(name);
	if (it != particleGroups_.end()) {

		ParticleGroup& group = it->second;

		for (uint32_t index = 0; index < parameter.count; ++index) {

			ParticleData particle{};

			if (group.behavior->GetType() == behaiviorType) {

				group.behavior->Create(particle, parameter);
			} else {

				assert(false && "behavior type of the existing particle group does not match");
			}

			group.particles.push_back(particle);
		}
	}
}

void ParticleSystem::PresetEmitParticle(
	const std::string& name, ParticleBehaviorType behaiviorType, const ParticleParameter& parameter) {

	// 既存のパーティクルグループを検索
	auto it = particleGroups_.find(name);
	if (it != particleGroups_.end()) {

		ParticleGroup& group = it->second;

		if (group.behavior->GetType() == behaiviorType) {

			std::vector<ParticleData> particles{};

			particleGroups_[name].behavior->PresetCreate(particles, parameter);
			particleGroups_[name].particles.resize(particles.size());
			std::copy(particles.begin(), particles.end(), particleGroups_[name].particles.begin());
		} else {

			assert(false && "behavior type of the existing particle group does not match");
		}
	}
}