#pragma once

// DXLib
#include "DXConstBuffer.h"

// C++
#include <string>

/*////////////////////////////////////////////////////////////////////////////////
							MaterialObject3D Class
////////////////////////////////////////////////////////////////////////////////*/
class MaterialObject3D :
	public DXConstBuffer<Material3D> {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	// default
	MaterialObject3D() = default;
	~MaterialObject3D() = default;

	// Main
	void Initialize();
	void Update();

public:
	//===================================================================*/
	//							public Variables
	//===================================================================*/

	Vector4 color;
	Material3D properties;
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

	// default
	MaterialObject2D() = default;
	~MaterialObject2D() = default;

	// Main
	void Initialize();
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

	// default
	OffscreenDepthMaterial() = default;
	~OffscreenDepthMaterial() = default;

	// Main
	void Initialize();
	void Update();

public:
	//===================================================================*/
	//							public Variables
	//===================================================================*/

	DepthMaterial property;
};

/*////////////////////////////////////////////////////////////////////////////////
							OffscreenDissolveMaterial Class
////////////////////////////////////////////////////////////////////////////////*/
class OffscreenDissolveMaterial :
	public DXConstBuffer<DissolveMaterial> {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	// default
	OffscreenDissolveMaterial() = default;
	~OffscreenDissolveMaterial() = default;

	// Main
	void Initialize();
	void Update();

public:
	//===================================================================*/
	//							public Variables
	//===================================================================*/

	DissolveMaterial properties;
	std::string textureName_;
};