#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Lib/Math/Vector3.h"

// json
#include <Externals/nlohmann/json.hpp>
// using
using Json = nlohmann::json;

// c++
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <cassert>

/*////////////////////////////////////////////////////////////////////////////////
*									JsonAdapter Class
////////////////////////////////////////////////////////////////////////////////*/
class JsonAdapter {
public:
	//===================================================================*/
	//							public Methods
	//===================================================================*/

	JsonAdapter() = default;
	~JsonAdapter() = default;

	static void Save(const std::string& saveDirectoryFilePath, const Json& jsonData);

	static Json Load(const std::string& loadDirectoryFilePath);

	static Json FromVector3(const Vector3& v);
	static Vector3 ToVector3(const Json& data);

private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

	static const std::string& baseDirectoryFilePath_;

};