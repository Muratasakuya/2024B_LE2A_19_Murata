#pragma once

//===================================================================*/
//	include
//===================================================================*/
#include "Game/3D/Base/IBaseGameObject.h"

// c++
#include <vector>

/*////////////////////////////////////////////////////////////////////////////////
*	MeshRenderer Class
////////////////////////////////////////////////////////////////////////////////*/
class MeshRenderer {
public:
	//===================================================================*/
	//	public Methods
	//===================================================================*/

	MeshRenderer() = default;
	~MeshRenderer() = default;

	static void Render();

	static void SetGameObject(IBaseGameObject* gameObject);

	static void EraseGameObject(IBaseGameObject* gameObject);
		
	static void Clear();

	static void SelectImGui();

	//* getter *//

	static std::vector<IBaseGameObject*> GetRenderObject() { return gameObjects_; };

private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

	//===================================================================*/
	///* variables

	static std::vector<IBaseGameObject*> gameObjects_;

	static IBaseGameObject* selectedGameObject_;

	static int currentObjectIndex_;

	//===================================================================*/
	///* functions

};