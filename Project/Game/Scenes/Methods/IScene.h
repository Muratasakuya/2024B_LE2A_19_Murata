#pragma once

// シーンの種類
enum SceneNo {

	TITLE,
	TUTORIAL,
	SELECT,
	GAME,
};

/*////////////////////////////////////////////////////////////////////////////////
*								IScene Class
////////////////////////////////////////////////////////////////////////////////*/
class IScene {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	IScene() = default;
	virtual ~IScene() = default;

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	// getter

	SceneNo GetSceneNo() const;

protected:
	//===================================================================*/
	//							private Variables
	//===================================================================*/

	// シーン
	static SceneNo sceneNo_;

};