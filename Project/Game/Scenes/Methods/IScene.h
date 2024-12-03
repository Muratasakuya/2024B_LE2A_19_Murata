#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Game/Methods/Transition.h"

// c++
#include <cstdint>
#include <memory>
#include <string>

/*////////////////////////////////////////////////////////////////////////////////
*								IScene Class
////////////////////////////////////////////////////////////////////////////////*/
class IScene {
public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	IScene() = default;
	virtual ~IScene() = default;

	virtual void Run() = 0;

	virtual void Init() = 0;

	virtual void Update() = 0;

	virtual void Cleanup() = 0;

	virtual std::string GetSceneName() const { return sceneName_; }

	virtual Transition* GetTransition() const { return transitionScene_.get(); }

protected:
	//===================================================================*/
	//							protected Methods
	//===================================================================*/

	//===================================================================*/
	///* variables

	static std::string sceneName_;

	static std::unique_ptr<Transition> transitionScene_;

};