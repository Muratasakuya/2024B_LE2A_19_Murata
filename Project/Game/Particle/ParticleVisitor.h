#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Lib/Structure.h"

// c++
#include <vector>
#include <optional>

//* front
class ParticleParameter;

/*////////////////////////////////////////////////////////////////////////////////
*						ParticleParameter Class
////////////////////////////////////////////////////////////////////////////////*/
class ParticleVisitor {
public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	ParticleVisitor() = default;
	virtual ~ParticleVisitor() = default;

	virtual void Visit(std::list<ParticleData>& particles, ParticleParameter& parameter) = 0;
	virtual void CreateUniformParticles(std::list<ParticleData>& particles, ParticleParameter& parameter) = 0;
	virtual void CreateNonUniformParticles(std::list<ParticleData>& particles, ParticleParameter& parameter) = 0;

	Vector4 SettingColor(const std::optional<Vector4>& color, const std::optional<RandomParticleColor>& randomParticleColor);

};

//================================================================================
//							DispersionVisitor
//================================================================================
class DispersionVisitor
	: public ParticleVisitor {
public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	DispersionVisitor() = default;
	~DispersionVisitor() = default;

	void Visit(std::list<ParticleData>& particles, ParticleParameter& parameter) override;
	void CreateUniformParticles(std::list<ParticleData>& particles, ParticleParameter& parameter) override;
	void CreateNonUniformParticles(std::list<ParticleData>& particles, ParticleParameter& parameter) override;

};

//================================================================================
//								ChaseVisitor
//================================================================================
class ChaseVisitor
	: public ParticleVisitor {
public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	ChaseVisitor() = default;
	~ChaseVisitor() = default;

	void Visit(std::list<ParticleData>& particles, ParticleParameter& parameter) override;
	void CreateUniformParticles(std::list<ParticleData>& particles, ParticleParameter& parameter) override;
	void CreateNonUniformParticles(std::list<ParticleData>& particles, ParticleParameter& parameter) override;

};

//================================================================================
//								ConvergeVisitor
//================================================================================
class ConvergeVisitor
	: public ParticleVisitor {
public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	ConvergeVisitor() = default;
	~ConvergeVisitor() = default;

	void Visit(std::list<ParticleData>& particles, ParticleParameter& parameter) override;
	void CreateUniformParticles(std::list<ParticleData>& particles, ParticleParameter& parameter) override;
	void CreateNonUniformParticles(std::list<ParticleData>& particles, ParticleParameter& parameter) override;

};

//================================================================================
//								InjectionVisitor
//================================================================================
class InjectionVisitor
	: public ParticleVisitor {
public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	InjectionVisitor() = default;
	~InjectionVisitor() = default;

	void Visit(std::list<ParticleData>& particles, ParticleParameter& parameter) override;
	void CreateUniformParticles(std::list<ParticleData>& particles, ParticleParameter& parameter) override;
	void CreateNonUniformParticles(std::list<ParticleData>& particles, ParticleParameter& parameter) override;

};