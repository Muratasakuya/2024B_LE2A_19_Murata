#include "IBaseParticle.h"

#include "Engine/Base/NewMoonGame.h"

/*////////////////////////////////////////////////////////////////////////////////
*						IBaseParticle classMethods
////////////////////////////////////////////////////////////////////////////////*/

void IBaseParticle::SetName(const std::string& name) {
	name_ = name;
	NewMoonGame::SetToImGui(this);
}

std::string IBaseParticle::GetName() const {
	return name_;
}