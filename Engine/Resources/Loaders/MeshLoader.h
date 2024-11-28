#pragma once

#include "Resources/Loaders/IResourceLoader.hpp"
#include "Resources/Mesh.hpp"

struct FileHandle;
struct SGeometryConfig;
struct SMaterialConfig;

namespace tinygltf {
	class Model;
}

class MeshLoader : public IResourceLoader {
public:
	MeshLoader();

public:
	virtual bool Load(const std::string& name, void* params, Resource* resource) override;
	virtual void Unload(Resource* resource) override;

private:
	virtual bool ImportObjFile(FileHandle* obj_file, const char* out_dsm_filename, std::vector<SGeometryConfig>& out_geometries);
	virtual void ProcessSubobject(std::vector<Vector3>& positions, std::vector<Vector3>& normals, std::vector<Vector2f>& texcoords, std::vector<MeshFaceData>& faces, SGeometryConfig* out_data);
	virtual bool ImportObjMaterialLibraryFile(const char* mtl_file_path);

	virtual bool LoadDsmFile(FileHandle* dsm_file, std::vector<SGeometryConfig>& out_geometries);
	virtual bool WriteDsmFile(const char* path, const char* name, std::vector<SGeometryConfig>& geometries);
	virtual bool WriteDmtFile(const char* mtl_file_path, SMaterialConfig* config);

	virtual bool ImportGltfFile(const std::string& obj_file, const char* out_dsm_filename, std::vector<SGeometryConfig>& out_geometries);
	virtual bool ProcessGltfMesh(size_t meshIndex, const tinygltf::Model& model, const std::vector<SMaterialConfig>& materialConfigs, std::vector<SGeometryConfig>& out_geometries);
	virtual bool ProcessGltfMaterial(const tinygltf::Model& model, const char* out_dsm_filename, std::vector<SMaterialConfig>& materialConfigs);

	virtual bool DeduplicateGeometry(std::vector<SGeometryConfig>& out_geometries);
};
