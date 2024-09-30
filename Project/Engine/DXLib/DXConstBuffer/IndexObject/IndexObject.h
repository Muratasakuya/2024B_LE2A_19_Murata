#pragma once

// DXLib
#include "DXConstBuffer.h"

// c++
#include <cstdint>

/*////////////////////////////////////////////////////////////////////////////////
*								IndexObject Class
////////////////////////////////////////////////////////////////////////////////*/
class IndexObject :
	public DXConstBuffer<uint32_t> {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	// default
	IndexObject() = default;
	~IndexObject() = default;

	void Initialize(UINT indexNum);
	void Update();

public:
	/*-----------------------------*/
	///			public変数
	/*-----------------------------*/

	std::vector<uint32_t> data;

};