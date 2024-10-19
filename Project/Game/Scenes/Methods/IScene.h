#pragma once

// c++
#include <cstdint>
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

};