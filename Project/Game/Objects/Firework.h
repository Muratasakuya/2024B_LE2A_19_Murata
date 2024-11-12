#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include <Game/Particle/BaseParticle.h>

// c++
#include <memory>
#include <vector>
#include <optional>

//* front
class RailEditor;

/*////////////////////////////////////////////////////////////////////////////////
*							Firework Class
////////////////////////////////////////////////////////////////////////////////*/
class Firework
	:public BaseParticle<kDispersion> {
public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	Firework() = default;
	~Firework() = default;

	void Init() override;

	void Update() override;

	void DerivedImGui() override;

	// Setter
	void SetColor(const Vector4& color) { parameter_.color = color; };
	void SetPos(const Vector3& pos) { parameter_.translate = pos; };

private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

};

/*////////////////////////////////////////////////////////////////////////////////
*							Firework Class
////////////////////////////////////////////////////////////////////////////////*/
class FireworkCollection {
public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	FireworkCollection() = default;
	~FireworkCollection() = default;

	void Init(RailEditor* railEditor);

	void Update();

	void FirstEmit();
	void SecondEmit();

	void Draw();

private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

	RailEditor* railEditor_ = nullptr;

	std::vector<std::unique_ptr<Firework>> fireworks_;

	std::array<std::optional<bool>, 2> emitEnable_;

};