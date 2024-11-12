#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Game/2D/Sprite.h"

// c++
#include <memory>

/*////////////////////////////////////////////////////////////////////////////////
*								RailScore Class
////////////////////////////////////////////////////////////////////////////////*/
class RailScore {
public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	RailScore() = default;
	~RailScore() = default;

	void Init();

	void Update();

	void Draw();

	void ImGui();

	// Setter
	void SetScore(float score);

private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

	std::unique_ptr<Sprite> score_;
	Vector2 scorePos_;

	float scoreValue_;

	std::array<std::unique_ptr<Sprite>, 4> scoreNumbers_;

	std::unique_ptr<Sprite> scoreBack_;
	Vector2 scoreBackPos_;

};