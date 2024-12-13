#pragma once

#include "Resource.hpp"
#include "Geometry.hpp"
#include "Math/Transform.hpp"
#include <vector>

enum MeshFileType {
	eMesh_File_Type_Not_Found,
	eMesh_File_Type_DSM,
	eMesh_File_Type_OBJ,
	eMesh_File_Type_GLTF
};

struct SupportedMeshFileType {
	const char* extension = nullptr;
	MeshFileType type;
	bool is_binary;
};

struct MeshVertexIndexData {
	uint32_t position_index;
	uint32_t normal_index;
	uint32_t texcoord_index;
};

struct MeshFaceData {
	MeshVertexIndexData vertices[3];
};

struct MeshGroupData {
	std::vector<MeshFaceData> Faces;
};

class Mesh {
public:
	Mesh() : geometries(nullptr), Parent(nullptr), geometry_count(0), UniqueID(INVALID_ID), Generation(INVALID_ID_U8){}
	virtual ~Mesh() { Unload(); }
	DAPI bool LoadFromResource(const std::string& resource_name);
	DAPI void Unload();

	DAPI Matrix4 GetLocal();
	DAPI Matrix4 GetWorldTransform();

	DAPI void SetParent(Mesh* parent) { Parent = parent; }
	DAPI void AttachTo(Mesh* parent) { Parent = parent; }
	DAPI Mesh* GetParent() const { return Parent; }

private:
	void LoadJobSuccess(void* params);
	void LoadJobFail(void* params);
	bool LoadJobStart(void* params, void* result_data);

public:
	std::string Name;
	uint32_t UniqueID;
	unsigned char Generation;
	unsigned short geometry_count;
	Geometry** geometries;
	Transform Transform;
	Mesh* Parent;
};

struct MeshLoadParams {
	std::string resource_name;
	Mesh* out_mesh = nullptr;
	class Resource mesh_resource;
};
