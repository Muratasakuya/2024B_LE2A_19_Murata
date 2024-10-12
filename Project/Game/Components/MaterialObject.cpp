#include "MaterialObject.h"

#include "Engine/Base/NewMoon.h"

/*////////////////////////////////////////////////////////////////////////////////
*								PrimitiveMaterial
////////////////////////////////////////////////////////////////////////////////*/
void PrimitiveMaterial::Init() {

	color.SetInit(1.0f);
	DXConstBuffer::Init(NewMoon::GetDXDevice());
}

void PrimitiveMaterial::Update() {

	DXConstBuffer::TransferData(color);
}

/*////////////////////////////////////////////////////////////////////////////////
*								MaterialObject3D
////////////////////////////////////////////////////////////////////////////////*/
void MaterialObject3D::Init() {

	color.SetInit(1.0f);
	properties.Init();
	DXConstBuffer::Init(NewMoon::GetDXDevice());
}

void MaterialObject3D::Update() {

	properties.color = color;
	TransferData(properties);
}

/*////////////////////////////////////////////////////////////////////////////////
*								MaterialObject2D
////////////////////////////////////////////////////////////////////////////////*/
void MaterialObject2D::Init() {

	color.SetInit(1.0f);
	properties.Init();
	DXConstBuffer::Init(NewMoon::GetDXDevice());
}

void MaterialObject2D::Update() {

	properties.color = color;
	TransferData(properties);
}

/*////////////////////////////////////////////////////////////////////////////////
*								OffscreenDepthMaterial
////////////////////////////////////////////////////////////////////////////////*/
void OffscreenDepthMaterial::Init() {

	DXConstBuffer::Init(NewMoon::GetDXDevice());
}

void OffscreenDepthMaterial::Update() {

	TransferData(property);
}

/*////////////////////////////////////////////////////////////////////////////////
*								OffscreenDissolveMaterial
////////////////////////////////////////////////////////////////////////////////*/
void OffscreenDissolveMaterial::Init() {

	properties.threshold = 0.0f;
	properties.edgeSize = 0.03f;
	properties.edgeColor = { 1.0f,0.4f,0.3f };
	DXConstBuffer::Init(NewMoon::GetDXDevice());
}

void OffscreenDissolveMaterial::Update() {

	TransferData(properties);
}