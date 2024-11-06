#include "ParticleParameter.h"

#include "Engine/Base/NewMoonGame.h"

//* front
#include "ParticleVisitor.h"
#include "ParticleSystem.h"

/*////////////////////////////////////////////////////////////////////////////////
*						ParticleParameter ClassMethods
////////////////////////////////////////////////////////////////////////////////*/

//================================================================================
//							DispersionParticleParameter
//================================================================================
void DispersionParticleParameter::Accept(ParticleVisitor& visitor, std::list<ParticleData>& particles) {
	visitor.Visit(particles, *this);
}

//================================================================================
//								ChaseParticleParameter
//================================================================================
void ChaseParticleParameter::Accept(ParticleVisitor& visitor, std::list<ParticleData>& particles) {
	visitor.Visit(particles, *this);
}

//================================================================================
//								ConvergeParticleParameter
//================================================================================
void ConvergeParticleParameter::Accept(ParticleVisitor& visitor, std::list<ParticleData>& particles) {
	visitor.Visit(particles, *this);
}