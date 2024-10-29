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

void ParticleSystem::Update() {

	Matrix4x4 backToFrontMatrix = Matrix4x4::MakeYawMatrix(std::numbers::pi_v<float>);
	Matrix4x4 billboardMatrix =
		Matrix4x4::Multiply(backToFrontMatrix, NewMoonGame::GameCamera()->GetCamera3D()->GetCameraMatrix());
	billboardMatrix.m[3][0] = 0.0f;
	billboardMatrix.m[3][1] = 0.0f;
	billboardMatrix.m[3][2] = 0.0f;

	for (auto& [name, group] : particleGroups_) {

		auto& particles = group.particles;
		group.particleBuffer.properties.resize(particles.size());

		for (auto it = particles.begin(); it != particles.end();) {

			// 生存時間外になったときに削除
			if (it->lifeTime <= it->currentTime) {
				it = particles.erase(it);
				continue;
			}

			// translate+=velocity
			it->transform.translate += {
				it->velocity.x* NewMoonGame::GetDeltaTime(),
					it->velocity.y* NewMoonGame::GetDeltaTime(),
					it->velocity.z* NewMoonGame::GetDeltaTime()
			};

			if (group.type_ == ParticleType::Dispersion) {

				it->transform.scale.SetInit(1.0f - (it->currentTime / it->lifeTime));
			} else if (group.type_ == ParticleType::Chase) {

				it->transform.scale.SetInit(std::clamp(it->currentTime / it->lifeTime, 0.0f, 0.15f));
			}

			// deltaTime++
			it->currentTime += NewMoonGame::GetDeltaTime();

			// world
			Matrix4x4 scaleMatrix = Matrix4x4::MakeScaleMatrix(it->transform.scale);
			Matrix4x4 translateMatrix = Matrix4x4::MakeTranslateMatrix(it->transform.translate);
			Matrix4x4 worldMatrix = scaleMatrix * billboardMatrix * translateMatrix;

			// wvp
			Matrix4x4 wvpMatrix = worldMatrix * NewMoonGame::GameCamera()->GetCamera3D()->GetViewProjectionMatrix();

			float alpha = 1.0f - (it->currentTime / it->lifeTime);

			int index = static_cast<uint32_t>(std::distance(particles.begin(), it));

			if (index < group.particleBuffer.properties.size()) {
				group.particleBuffer.properties[index].World = worldMatrix;
				group.particleBuffer.properties[index].WVP = wvpMatrix;
				group.particleBuffer.properties[index].color = it->color;
				group.particleBuffer.properties[index].color.w = alpha;
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

void ParticleSystem::CreateDispersionParticle(
	const std::string& modelName,
	const std::string& name, const Vector3& centerPos, uint32_t emitCount,
	std::optional<float> speed, std::optional<float> lifeTime, const std::optional<Vector4>& color) {

	assert(particleGroups_.find(name) == particleGroups_.end() && "Particle group with this name already exists");

	ParticleGroup newGroup;
	for (uint32_t index = 0; index < emitCount; ++index) {

		ParticleData particle{};

		// 固定
		particle.transform.scale.SetInit(1.0f);
		particle.transform.rotate.SetInit(0.0f);
		particle.currentTime = 0.0f;

		particle.transform.translate = centerPos;

		if (!speed) {

			particle.velocity =
				Vector3(Random::Generate(-1.0f, 1.0f), Random::Generate(-1.0f, 1.0f), Random::Generate(-1.0f, 1.0f));
		} else {

			particle.velocity =
				Vector3(Random::Generate(
					-speed.value(), speed.value()), Random::Generate(-speed.value(),
						speed.value()), Random::Generate(-speed.value(), speed.value())
				);
		}

		particle.color =
			color.value_or(Vector4(Random::Generate(0.0f, 1.0f), Random::Generate(0.0f, 1.0f), Random::Generate(0.0f, 1.0f), 1.0f));

		particle.lifeTime = lifeTime.value_or(Random::Generate(1.0f, 3.0f));

		newGroup.particles.push_back(particle);
	}

	particleGroups_[name].type_ = ParticleType::Dispersion;

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

void ParticleSystem::EmitDispersionParticle(
	const std::string& name, const Vector3& centerPos, uint32_t emitCount,
	std::optional<float> speed, std::optional<float> lifeTime, const std::optional<Vector4>& color) {

	// 既存のパーティクルグループを検索
	auto it = particleGroups_.find(name);
	if (it != particleGroups_.end()) {

		ParticleGroup& group = it->second;

		group.type_ = ParticleType::Dispersion;

		for (uint32_t index = 0; index < emitCount; ++index) {

			ParticleData particle{};

			// 固定
			particle.transform.scale.SetInit(1.0f);
			particle.transform.rotate.SetInit(0.0f);
			particle.currentTime = 0.0f;

			particle.transform.translate = centerPos;

			if (!speed) {

				particle.velocity =
					Vector3(Random::Generate(-1.0f, 1.0f), Random::Generate(-1.0f, 1.0f), Random::Generate(-1.0f, 1.0f));
			} else {

				particle.velocity =
					Vector3(Random::Generate(
						-speed.value(), speed.value()), Random::Generate(-speed.value(),
							speed.value()), Random::Generate(-speed.value(), speed.value())
					);
			}

			particle.color =
				color.value_or(Vector4(Random::Generate(0.0f, 1.0f), Random::Generate(0.0f, 1.0f), Random::Generate(0.0f, 1.0f), 1.0f));

			particle.lifeTime = lifeTime.value_or(Random::Generate(1.0f, 3.0f));

			group.particles.push_back(particle);
		}
	}
}

void ParticleSystem::CreateChaseParticle(
	const std::string& modelName, const std::string& name,
	const Vector3& currentPos, const Vector3& prePos, uint32_t emitCount,
	std::optional<float> speed, std::optional<float> lifeTime, const std::optional<Vector4>& color) {

	assert(particleGroups_.find(name) == particleGroups_.end() && "Particle group with this name already exists");

	ParticleGroup newGroup;
	for (uint32_t index = 0; index < emitCount; ++index) {

		ParticleData particle{};

		// 固定
		particle.transform.scale.Init();
		particle.transform.rotate.SetInit(0.0f);
		particle.currentTime = 0.0f;
		particle.easedT_ = std::nullopt;

		particle.transform.translate = currentPos;

		Vector3 direction = Vector3::Normalize(currentPos - prePos);

		Vector3 randomOffset{};
		randomOffset.SetInit(Random::Generate(-0.4f, 0.4f));
		randomOffset.z = 0.0f;
		Vector3 spreadDirection = Vector3::Normalize(direction + randomOffset);

		if (!speed) {

			particle.velocity = Random::Generate(0.4f, 0.8f) * spreadDirection;
		} else {

			particle.velocity = speed.value() * spreadDirection;
		}

		particle.color =
			color.value_or(Vector4(Random::Generate(0.0f, 1.0f), Random::Generate(0.0f, 1.0f), Random::Generate(0.0f, 1.0f), 1.0f));

		particle.lifeTime = lifeTime.value_or(Random::Generate(1.0f, 2.0f));

		newGroup.particles.push_back(particle);
	}

	particleGroups_[name].type_ = ParticleType::Chase;
	particleGroups_[name].easingType_ = EasingType::EaseOutQuart;

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

void ParticleSystem::EmitChaseParticle(
	const std::string& name, const Vector3& currentPos, const Vector3& prePos, uint32_t emitCount,
	std::optional<float> speed, std::optional<float> lifeTime, const std::optional<Vector4>& color) {

	// 既存のパーティクルグループを検索
	auto it = particleGroups_.find(name);
	if (it != particleGroups_.end()) {

		ParticleGroup& group = it->second;

		group.type_ = ParticleType::Chase;
		group.easingType_ = EasingType::EaseOutQuart;

		for (uint32_t index = 0; index < emitCount; ++index) {

			ParticleData particle{};

			// 固定
			particle.transform.scale.Init();
			particle.transform.rotate.SetInit(0.0f);
			particle.currentTime = 0.0f;
			particle.easedT_ = std::nullopt;

			particle.transform.translate = currentPos;

			Vector3 direction = Vector3::Normalize(currentPos - prePos);

			Vector3 randomOffset{};
			randomOffset.SetInit(Random::Generate(-0.4f, 0.4f));
			randomOffset.z = 0.0f;
			Vector3 spreadDirection = Vector3::Normalize(direction + randomOffset);

			if (!speed) {

				particle.velocity = Random::Generate(0.4f, 0.8f) * spreadDirection;
			} else {

				particle.velocity = speed.value() * spreadDirection;
			}

			particle.color =
				color.value_or(Vector4(Random::Generate(0.0f, 1.0f), Random::Generate(0.0f, 1.0f), Random::Generate(0.0f, 1.0f), 1.0f));

			particle.lifeTime = lifeTime.value_or(Random::Generate(1.0f, 2.0f));

			if (currentPos == prePos) {
				particle.lifeTime = 0.0f;
			}

			group.particles.push_back(particle);
		}
	}
}