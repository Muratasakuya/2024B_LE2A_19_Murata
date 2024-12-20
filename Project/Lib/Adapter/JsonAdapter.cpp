#include "JsonAdapter.h"

/*////////////////////////////////////////////////////////////////////////////////
*						JsonManager classMethods
////////////////////////////////////////////////////////////////////////////////*/

const std::string& JsonAdapter::baseDirectoryFilePath_ = "./Resources/Json/";

void JsonAdapter::Save(const std::string& saveDirectoryFilePath, const Json& jsonData) {

	std::string fullPath = baseDirectoryFilePath_ + saveDirectoryFilePath;
	std::filesystem::path path(fullPath);

	if (path.extension() != ".json") {

		path.replace_extension(".json");
		fullPath = path.string();
	}

	if (!std::filesystem::exists(path.parent_path())) {
		std::filesystem::create_directories(path.parent_path());
	}

	std::ofstream file(fullPath);

	if (!file.is_open()) {

		assert(false && "failed to open file for saving json");
		return;
	}

	file << jsonData.dump(4);
	file.close();
}

Json JsonAdapter::Load(const std::string& loadDirectoryFilePath) {

	std::string fullPath = baseDirectoryFilePath_ + loadDirectoryFilePath;

	std::ifstream file(fullPath);

	if (!file.is_open()) {

		assert(false && "failed to open file for loading json");
		return Json();
	}

	Json jsonData;
	file >> jsonData;
	file.close();

	return jsonData;
}

Json JsonAdapter::FromVector3(const Vector3& v) {

	return Json{
		{"x", v.x},
		{"y", v.y},
		{"z", v.z}
	};
}

Vector3 JsonAdapter::ToVector3(const Json& data) {

	Vector3 v = Vector3::Zero();
	if (data.contains("x") && data.contains("y") && data.contains("z")) {
		v.x = data["x"].get<float>();
		v.y = data["y"].get<float>();
		v.z = data["z"].get<float>();
	}
	return v;
}

Json JsonAdapter::FromVector4(const Vector4& v) {

	return Json{
		{"x", v.x},
		{"y", v.y},
		{"z", v.z},
		{"w", v.w}
	};
}

Vector4 JsonAdapter::ToVector4(const Json& data) {

	Vector4 v{};
	if (data.contains("x") && data.contains("y") && data.contains("z") && data.contains("w")) {
		v.x = data["x"].get<float>();
		v.y = data["y"].get<float>();
		v.z = data["z"].get<float>();
		v.w = data["w"].get<float>();
	}
	return v;
}
