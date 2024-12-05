#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Engine/MyDirectXClass/Pipeline/PipelineStateStructure.h"

// c++
#include <string>

//===================================================================*/
//							interface Methods
//===================================================================*/
class IBaseParticle {
public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	IBaseParticle() = default;
	virtual ~IBaseParticle() = default;

	virtual void Init() = 0;

	virtual void Update() = 0;

	virtual void Draw(BlendMode blendMode = BlendMode::kBlendModeNormal) = 0;

	virtual void ImGui() = 0;

	// Setter
	virtual void SetName(const std::string& name = "particle");

	// Getter
	virtual std::string GetName() const;

private:
	//===================================================================*/
	//							protected Methods
	//===================================================================*/

	std::string name_;

};