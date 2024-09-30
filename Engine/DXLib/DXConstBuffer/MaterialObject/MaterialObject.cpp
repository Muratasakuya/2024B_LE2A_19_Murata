#include "MaterialObject.h"

#include "NewMoon.h"

/*////////////////////////////////////////////////////////////////////////////////
*								MaterialObject3D
////////////////////////////////////////////////////////////////////////////////*/
void MaterialObject3D::Initialize() {

	// default
	color.SetInitialize(1.0f);
	properties.Initialize();

	// 定数バッファ生成
	DXConstBuffer::Initialize(NewMoon::GetDXDevice());
}

void MaterialObject3D::Update() {

	properties.color = color;

	// 定数バッファにデータを転送
	TransferData(properties);
}

/*////////////////////////////////////////////////////////////////////////////////
*								MaterialObject2D
////////////////////////////////////////////////////////////////////////////////*/
void MaterialObject2D::Initialize() {

	// default
	color.SetInitialize(1.0f);
	properties.Initialize();

	// 定数バッファ生成
	DXConstBuffer::Initialize(NewMoon::GetDXDevice());
}

void MaterialObject2D::Update() {

	properties.color = color;

	// 定数バッファにデータを転送
	TransferData(properties);
}

/*////////////////////////////////////////////////////////////////////////////////
*								OffscreenDepthMaterial
////////////////////////////////////////////////////////////////////////////////*/
void OffscreenDepthMaterial::Initialize() {

	// 定数バッファ生成
	DXConstBuffer::Initialize(NewMoon::GetDXDevice());
}

void OffscreenDepthMaterial::Update() {

	// 定数バッファにデータを転送
	TransferData(property);
}

/*////////////////////////////////////////////////////////////////////////////////
*								OffscreenDissolveMaterial
////////////////////////////////////////////////////////////////////////////////*/
void OffscreenDissolveMaterial::Initialize() {

	// default
	properties.threshold = 0.0f;
	properties.edgeSize = 0.03f;
	properties.edgeColor = { 1.0f,0.4f,0.3f };

	// 定数バッファ生成
	DXConstBuffer::Initialize(NewMoon::GetDXDevice());
}

void OffscreenDissolveMaterial::Update() {

	// 定数バッファにデータを転送
	TransferData(properties);
}