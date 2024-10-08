#pragma once

// シーンの種類
enum SceneNo {

	TITLE,
	TUTORIAL,
	SELECT,
	GAME,
	RESULT
};

// ソロ マルチ選択
enum class GameMode {

	SOLO,
	MATCH
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

	virtual void Initialize() = 0;
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