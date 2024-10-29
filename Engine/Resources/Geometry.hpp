#pragma once

#include "Math/MathTypes.hpp"

class Material;

class Geometry {
public:
	DAPI void SetName(const std::string& n) { name = n; }
	DAPI std::string GetName() { return name; }

public:
	uint32_t ID;
	uint32_t InternalID;
	uint32_t Generation;
	Vec3 Center;
	Extents3D Extents;
	std::string name;
	Material* Material = nullptr;

};