#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Engine/MyDirectXClass/DXConstBuffer.h"

// C++
#include <string>

/*////////////////////////////////////////////////////////////////////////////////
							PrimitiveMaterial Class
////////////////////////////////////////////////////////////////////////////////*/
class PrimitiveMaterial :
	public DXConstBuffer<Vector4> {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	PrimitiveMaterial() = default;
	~PrimitiveMaterial() = default;

	void Init();
	void Update();

public:
	//===================================================================*/
	//							public Variables
	//===================================================================*/

	Vector4 color;
};

/*////////////////////////////////////////////////////////////////////////////////
							MaterialObject3D Class
////////////////////////////////////////////////////////////////////////////////*/
class MaterialObject3D :
	public DXConstBuffer<Material3D> {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	MaterialObject3D() = default;
	~MaterialObject3D() = default;

	void Init();
	void Update();

	void SetCommand(ID3D12GraphicsCommandList* commandList);

public:
	//===================================================================*/
	//							public Variables
	//===================================================================*/

	Vector4 color;
	Material3D properties;

private:
	//===================================================================*/
	//							private Variables
	//===================================================================*/

	const UINT rootParameterIndex_ = 0;

};

/*////////////////////////////////////////////////////////////////////////////////
							MaterialObject2D Class
////////////////////////////////////////////////////////////////////////////////*/
class MaterialObject2D :
	public DXConstBuffer<Material2D> {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	MaterialObject2D() = default;
	~MaterialObject2D() = default;

	void Init();
	void Update();

public:
	//===================================================================*/
	//							public Variables
	//===================================================================*/

	Vector4 color;
	Material2D properties;
};

/*////////////////////////////////////////////////////////////////////////////////
							OffscreenDepthMaterial Class
////////////////////////////////////////////////////////////////////////////////*/
class OffscreenDepthMaterial :
	public DXConstBuffer<DepthMaterial> {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	OffscreenDepthMaterial() = default;
	~OffscreenDepthMaterial() = default;

	void Init();
	void Update();

public:
	//===================================================================*/
	//							public Variables
	//===================================================================*/

	DepthMaterial property;
};

/*////////////////////////////////////////////////////////////////////////////////
						DissolveMaterialBuffer Class
////////////////////////////////////////////////////////////////////////////////*/
class DissolveMaterialBuffer :
	public DXConstBuffer<DissolveMaterial> {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	DissolveMaterialBuffer() = default;
	~DissolveMaterialBuffer() = default;

	void Init();
	void Update();

public:
	//===================================================================*/
	//							public Variables
	//===================================================================*/

	DissolveMaterial properties;
	std::string textureName_;

};