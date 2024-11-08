#pragma once

#include "Math/MathTypes.hpp"
#include "Math/Transform.hpp"

class Material;

struct SGeometryConfig {
public:
	void SetMaterialName(const std::string& mn) { material_name = mn; }
	const std::string& GetMaterialName() const { return material_name; }
	void SetName(const std::string& n) { name = n; }
	const std::string& GetName() const { return name; }

	// Vertices
	uint32_t vertex_size = 0;
	uint32_t vertex_count = 0;
	void* vertices = nullptr;

	// Indices
	uint32_t index_size = 0;
	uint32_t index_count = 0;
	void* indices = nullptr;

	Vec3 center;
	Vec3 min_extents;
	Vec3 max_extents;

	std::string name;
	std::string material_name;
};

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