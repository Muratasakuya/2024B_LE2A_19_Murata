#include "ParticleSystem.h"

#include "Engine/Base/NewMoon.h"
#include "Engine/Base/NewMoonGame.h"
#include "Engine/Managers/SrvManager.h"
#include "Lib/Adapter/Random.h"

/*////////////////////////////////////////////////////////////////////////////////
*							ParticleSystem classMethods
////////////////////////////////////////////////////////////////////////////////*/

void ParticleSystem::CreateVertexData() {

	// 左下
	vertexDataArr_[0] =
	{ .pos = {1.0f,-1.0f,0.0f,1.0f},.texcoord = {0.0f,1.0f},.normal = {0.0f,0.0f,1.0f} };
	// 左上
	vertexDataArr_[1] =
	{ .pos = {1.0f,1.0f,0.0f,1.0f},.texcoord = {0.0f,0.0f},.normal = {0.0f,0.0f,1.0f} };
	// 右下
	vertexDataArr_[2] =
	{ .pos = {-1.0f,-1.0f,0.0f,1.0f},.texcoord = {1.0f,1.0f},.normal = {0.0f,0.0f,1.0f} };
	// 右上
	vertexDataArr_[3] =
	{ .pos = {-1.0f,1.0f,0.0f,1.0f},.texcoord = {1.0f,0.0f},.normal = {0.0f,0.0f,1.0f} };

	// ConstBuffer初期化
	vertex_.Init(static_cast<UINT>(kParticleModelVertexNum));
	index_.Init(static_cast<UINT>(kParticleModelIndexNum));

	vertex_.data.resize(kParticleModelVertexNum);
	std::copy(vertexDataArr_.begin(), vertexDataArr_.end(), vertex_.data.begin());

	// Indexの設定
	index_.data.resize(kParticleModelIndexNum);
	index_.data[0] = 0;
	index_.data[1] = 1;
	index_.data[2] = 2;
	index_.data[3] = 1;
	index_.data[4] = 3;
	index_.data[5] = 2;

	// ConstBuffer転送
	vertex_.Update();
	index_.Update();
}

void ParticleSystem::Init() {

	CreateVertexData();
}

void ParticleSystem::Update() {

	// ビルボード行列の計算
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

			it->transform.scale =
				Vector3(1.0f - (it->currentTime / it->lifeTime), 1.0f - (it->currentTime / it->lifeTime), 1.0f - (it->currentTime / it->lifeTime));

			// deltaTime++
			it->currentTime += NewMoonGame::GetDeltaTime();

			// world
			Matrix4x4 scaleMatrix = Matrix4x4::MakeScaleMatrix(it->transform.scale);
			Matrix4x4 translateMatrix = Matrix4x4::MakeTranslateMatrix(it->transform.translate);
			Matrix4x4 worldMatrix = scaleMatrix * billboardMatrix * translateMatrix;

			// wvp
			Matrix4x4 wvpMatrix = worldMatrix * NewMoonGame::GameCamera()->GetCamera3D()->GetViewProjectionMatrix();

			// どんどん薄くしていくなら
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

void ParticleSystem::Draw(const std::string& name, const std::string& textureName, BlendMode blendMode) {

	auto commandList = NewMoon::GetCommandList();

	NewMoon::SetGraphicsPipeline(commandList, pParticle, blendMode);
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList->IASetVertexBuffers(0, 1, &vertex_.GetVertexBuffer());
	commandList->IASetIndexBuffer(&index_.GetIndexBuffer());
	particleGroups_[name].particleBuffer.SetCommand(commandList, particleGroups_[name].particleBuffer.GetRootParameterIndex());
	NewMoon::SetGraphicsRootDescriptorTable(commandList, 1, textureName);
	commandList->SetGraphicsRootDescriptorTable(2, NewMoon::GetSrvManagerPtr()->GetGPUHandle(particleGroups_[name].instancingSrvIndex));
	commandList->DrawIndexedInstanced(static_cast<UINT>(index_.data.size()), particleGroups_[name].numInstance, 0, 0, 0);
}

void ParticleSystem::Emit(const std::string name, const Vector3& pos, uint32_t count) {

	// 既存のパーティクルグループを検索
	auto it = particleGroups_.find(name);
	if (it != particleGroups_.end()) {

		ParticleGroup& group = it->second;

		for (uint32_t i = 0; i < count; i++) {

			ParticleData particle;

			particle.transform.translate = pos;

			particle.transform.scale.SetInit(1.0f);

			particle.transform.rotate = { 1.0f, 1.0f, 1.0f };

			particle.velocity =
			{ Random::Generate(-1.5f,1.5f), Random::Generate(-1.5f,1.5f), Random::Generate(-1.5f,1.5f) };

			particle.color =
			{ Random::Generate(0.0f,1.0f), Random::Generate(0.0f,1.0f), Random::Generate(0.0f,1.0f),1.0f };

			particle.lifeTime = Random::Generate(0.5f, 2.0f);
			particle.currentTime = 0.0f;

			group.particles.push_back(particle);
		}
	}
}

void ParticleSystem::CreateParticleGroup(const Vector3& pos, const std::string name, const std::string& textureName, uint32_t count) {

	assert(particleGroups_.find(name) == particleGroups_.end() && "Particle group with this name already exists");

	ParticleGroup newGroup;
	for (uint32_t i = 0; i < count; i++) {

		ParticleData particle;

		particle.transform.translate = pos;

		particle.transform.scale.SetInit(1.0f);

		particle.transform.rotate = { 1.0f, 1.0f, 1.0f };

		particle.velocity =
		{ Random::Generate(-1.5f,1.5f), Random::Generate(-1.5f,1.5f), Random::Generate(-1.5f,1.5f) };

		particle.color =
		{ Random::Generate(0.0f,1.0f), Random::Generate(0.0f,1.0f), Random::Generate(0.0f,1.0f),1.0f };

		particle.lifeTime = Random::Generate(0.5f, 2.0f);
		particle.currentTime = 0.0f;

		newGroup.particles.push_back(particle);
	}
	particleGroups_[name] = newGroup;

	//!! alreadyLoadTexture !!//
	particleGroups_[name].material.textureName = textureName;

	//* CreateStructureBuffer *//
	particleGroups_[name].material.srvIndex = NewMoon::GetSrvManagerPtr()->Allocate();
	particleGroups_[name].numInstance = instanceMaxCount_;
	particleGroups_[name].particleBuffer.Init(particleGroups_[name].numInstance);
	particleGroups_[name].instancingSrvIndex = NewMoon::GetSrvManagerPtr()->Allocate();
	NewMoon::GetSrvManagerPtr()->CreateSRVForStructureBuffer(
		particleGroups_[name].instancingSrvIndex, particleGroups_[name].particleBuffer.GetResource(),
		particleGroups_[name].numInstance, sizeof(ParticleForGPU));
}
