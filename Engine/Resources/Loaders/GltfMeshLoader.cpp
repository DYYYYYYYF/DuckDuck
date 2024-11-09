#include "MeshLoader.h"
#include "Core/DMemory.hpp"
#include "Core/EngineLogger.hpp"

#include "Containers/TString.hpp"
#include "Platform/FileSystem.hpp"
#include "Systems/ResourceSystem.h"
#include "Systems/GeometrySystem.h"
#include "Math/GeometryUtils.hpp"

#include <vector>
#include <stdio.h>	//sscanf

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define JSON_NOEXCEPTION
#define TINYGLTF_NOEXCEPTION
#include <tiny_gltf.h>

bool MeshLoader::ImportGltfFile(const std::string& obj_file, const char* out_dsm_filename, std::vector<SGeometryConfig>& out_geometries) {
	tinygltf::Model model;
	tinygltf::TinyGLTF loader;
	std::string err;
	std::string warn;

	// 加载 GLTF 文件
	bool success = loader.LoadASCIIFromFile(&model, &err, &warn, obj_file);
	if (!success) {
		std::cerr << "Failed to load GLTF model: " << err << std::endl;
		return false;
	}

	// 遍历网格
	
	// Default name is filename.
	std::string name = obj_file;
	// Materials
	std::vector<SMaterialConfig> MaterialConfigs;
	// Groups
	std::vector<MeshGroupData> Groups;
	// Matrixs
	std::map<size_t, Matrix4> MeshMatrixMap;
	std::map<size_t, size_t> NodeMeshMap;

	ProcessGltfMaterial(model, obj_file.c_str(), MaterialConfigs);
	
	// TODO: Nodes(需要加载Nodes的Transform，稍复杂模型都需要矩阵信息)
	for (size_t i = 0; i < model.nodes.size(); ++i)
	{
		const auto& Node = model.nodes[i];
		Transform LocalTransform = Transform();
		if (!Node.matrix.empty()) {
			LocalTransform = Transform(Node.matrix);
		}
		else {
			// 计算变换矩阵，如果没有 matrix 数据，使用 scale、rotation、translation 生成
			if (!Node.scale.empty()) {
				Vec3 Scale = Vec3((float)Node.scale[0], (float)Node.scale[1], (float)Node.scale[2]);
				LocalTransform.SetScale(Scale);
			}
			if (!Node.rotation.empty()) {
				Quaternion rotation = Quaternion((float)Node.rotation[3], (float)Node.rotation[0], (float)Node.rotation[1], (float)Node.rotation[2]);
				LocalTransform.SetRotation(rotation);
			}
			if (!Node.translation.empty()) {
				Vec3 Translation = Vec3((float)Node.translation[0], (float)Node.translation[1], (float)Node.translation[2]);
				LocalTransform.SetPosition(Translation);
			}
		}

		MeshMatrixMap[i] = LocalTransform.GetLocal();

		for (const auto& child : Node.children) {
			if (MeshMatrixMap.find(child) == MeshMatrixMap.end()) {
				MeshMatrixMap[child] = Matrix4::Identity();
			}
			MeshMatrixMap[child] = MeshMatrixMap[i].Multiply(MeshMatrixMap[child]);
			NodeMeshMap[child] = i;
		}
	}

	// Meshes
	Transform DefaultLocalTransform = Transform();
	for (size_t im = 0; im < model.meshes.size(); ++im) {
		ProcessGltfMesh(im, model, MaterialConfigs, out_geometries);
	}

	MaterialConfigs.clear();
	Groups.clear();

	// De-duplicate geometry.
	DeduplicateGeometry(out_geometries);

	// Output a .dsm file, which will be loaded in the future.
	return WriteDsmFile(out_dsm_filename, name.c_str(), out_geometries);
}

bool MeshLoader::ProcessGltfMaterial(const tinygltf::Model& model, const char* out_dsm_filename, std::vector<SMaterialConfig>& materialConfigs) {
	// Materials
	for (size_t i = 0; i < model.materials.size(); ++i) {

		const auto& material = model.materials[i];
		SMaterialConfig CurrentConfig;
		if (material.name.empty()) {
			char file[512] = "";
			StringFilenameNoExtensionFromPath(file, out_dsm_filename);
			CurrentConfig.name = file + std::string("_auto_material_name_") + std::to_string(i);
		}
		else {
			CurrentConfig.name = material.name;
		}

		// 基础颜色
		if (material.values.find("baseColorFactor") != material.values.end()) {
			const auto& baseColorFactor = material.values.at("baseColorFactor").number_array;
			CurrentConfig.diffuse_color = Vec4((float)baseColorFactor[0], (float)baseColorFactor[1], (float)baseColorFactor[2], (float)baseColorFactor[3]);
		}

		// 金属度
		if (material.values.find("metallicFactor") != material.values.end()) {
			CurrentConfig.Metallic = (float)material.values.at("metallicFactor").Factor();
		}

		// 粗糙度
		if (material.values.find("roughnessFactor") != material.values.end()) {
			CurrentConfig.Roughness = (float)material.values.at("roughnessFactor").Factor();
		}

		// 粗糙度
		if (material.values.find("emissiveFactor") != material.values.end()) {
			const auto& emissiveColorFactor = material.values.at("baseColorFactor").number_array;
			CurrentConfig.EmissiveColor = Vec4((float)emissiveColorFactor[0], (float)emissiveColorFactor[1], (float)emissiveColorFactor[2], 1.0f);
		}

		// 基本颜色纹理贴图
		if (material.values.find("baseColorTexture") != material.values.end()) {
			int textureIndex = material.values.at("baseColorTexture").TextureIndex();
			const tinygltf::Texture& texture = model.textures[textureIndex];
			// 获取图像索引
			if (texture.source < 0 || texture.source >= model.images.size()) {
				LOG_WARN("GLTF laod warning.Invalid image index %i in material %s.", texture.source, CurrentConfig.name.c_str());
				continue;
			}

			if (texture.source != -1) {
				StringFilenameNoExtensionFromPath(CurrentConfig.diffuse_map_name, model.images[texture.source].uri.data());
			}
		}

		// 法线贴图
		if (material.values.find("normalTexture") != material.values.end()) {
			int textureIndex = material.values.at("normalTexture").TextureIndex();
			const tinygltf::Texture& texture = model.textures[textureIndex];
			// 获取图像索引
			if (texture.source < 0 || texture.source >= model.images.size()) {
				LOG_WARN("GLTF laod warning.Invalid image index %i in material %s.", texture.source, CurrentConfig.name.c_str());
				continue;
			}

			if (texture.source != -1) {
				StringFilenameNoExtensionFromPath(CurrentConfig.normal_map_name, model.images[texture.source].uri.data());
			}
		}

		// 金属度/粗糙度贴图
		if (material.values.find("metallicRoughnessTexture") != material.values.end()) {
			int textureIndex = material.values.at("metallicRoughnessTexture").TextureIndex();
			const tinygltf::Texture& texture = model.textures[textureIndex];
			// 获取图像索引
			if (texture.source < 0 || texture.source >= model.images.size()) {
				LOG_WARN("GLTF laod warning.Invalid image index %i in material %s.", texture.source, CurrentConfig.name.c_str());
				continue;
			}

			if (texture.source != -1) {
				char TexName[512] = "";
				StringFilenameNoExtensionFromPath(TexName, model.images[texture.source].uri.data());
				CurrentConfig.MetallicRoughnessTexName = TexName;
			}
		}

		// 自发光贴图
		if (material.values.find("emissiveFactor") != material.values.end()) {
			int textureIndex = material.values.at("emissiveFactor").TextureIndex();
			const tinygltf::Texture& texture = model.textures[textureIndex];
			// 获取图像索引
			if (texture.source < 0 || texture.source >= model.images.size()) {
				LOG_WARN("GLTF laod warning.Invalid image index %i in material %s.", texture.source, CurrentConfig.name.c_str());
				continue;
			}

			if (texture.source != -1) {
				char TexName[512] = "";
				StringFilenameNoExtensionFromPath(TexName, model.images[texture.source].uri.data());
				CurrentConfig.EmissiveFactorTexName = TexName;
			}
		}

		CurrentConfig.shader_name = "Shader.Builtin.World";

		materialConfigs.push_back(CurrentConfig);
		WriteDmtFile(out_dsm_filename, &CurrentConfig);
	}

	return true;
}

bool MeshLoader::ProcessGltfMesh(size_t meshIndex, const tinygltf::Model& model, const std::vector<SMaterialConfig>& materialConfigs, std::vector<SGeometryConfig>& out_geometries) {
	// Positions
	std::vector<Vec3> Positions;
	Positions.reserve(65535);
	// Normals
	std::vector<Vec3> Normals;
	Normals.reserve(65535);
	// Texcoords
	std::vector<Vec2> Texcoords;
	Texcoords.reserve(65535);
	// Tangents
	std::vector<Vec3> Tangents;
	Tangents.reserve(65535);
	// Indices
	std::vector<uint32_t> Indices;

	const auto& mesh = model.meshes[meshIndex];

	// 实际上是一个Geometry，看如何理解Geo和Pri
	MeshGroupData GroupData;
	for (const auto& primitive : mesh.primitives) {
		switch (primitive.mode)
		{
		case TINYGLTF_MODE_TRIANGLE_STRIP:
		{
			continue;
		}
		case TINYGLTF_MODE_TRIANGLES:
		{

		} break;
		default:
			continue;
		}

		// 顶点索引
		const tinygltf::Accessor& indexAccessor = model.accessors[primitive.indices];
		const tinygltf::BufferView& indexBufferView = model.bufferViews[indexAccessor.bufferView];
		const tinygltf::Buffer& indexBuffer = model.buffers[indexBufferView.buffer];
		int ByteStride = indexAccessor.ByteStride(indexBufferView);

		ASSERT(indexAccessor.count % 3 == 0);
		if (indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE) {
			ASSERT(ByteStride == 1);
			if (indexBuffer.data.empty() ||
				indexAccessor.byteOffset + indexBufferView.byteOffset + indexAccessor.count * sizeof(uint8_t) > indexBuffer.data.size()) {
				LOG_FATAL("Index buffer is invalid or out of bounds!");
				return false;
			}

			const uint8_t* buf = reinterpret_cast<const uint8_t*>(&(indexBuffer
				.data[indexAccessor.byteOffset + indexBufferView.byteOffset]));
			for (size_t index = 0; index < indexAccessor.count / 3; index++)
			{
				MeshFaceData MeshFace;
				for (size_t f = 0; f < 3; f++) {
					uint32_t DataIndex = static_cast<uint32_t>(buf[index + f]);
					MeshFace.vertices[f].position_index = DataIndex;
					MeshFace.vertices[f].texcoord_index = DataIndex;
					MeshFace.vertices[f].normal_index = DataIndex;
				}

				GroupData.Faces.push_back(MeshFace);
			}
		}
		else if (indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
			ASSERT(ByteStride == 2);
			if (indexBuffer.data.empty() ||
				indexAccessor.byteOffset + indexBufferView.byteOffset + indexAccessor.count * sizeof(uint16_t) > indexBuffer.data.size()) {
				LOG_FATAL("Index buffer is invalid or out of bounds!");
				return false;
			}

			const uint16_t* buf = reinterpret_cast<const uint16_t*>(&(indexBuffer.data[indexAccessor.byteOffset + indexBufferView.byteOffset]));
			for (size_t index = 0; index < indexAccessor.count / 3; index++)
			{
				MeshFaceData MeshFace;
				for (size_t f = 0; f < 3; f++) {
					uint32_t DataIndex = static_cast<uint32_t>(buf[index * 3 + f]);
					MeshFace.vertices[f].position_index = DataIndex;
					MeshFace.vertices[f].texcoord_index = DataIndex;
					MeshFace.vertices[f].normal_index = DataIndex;
				}

				GroupData.Faces.push_back(MeshFace);
			}
		}
		else if (indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT) {
			ASSERT(ByteStride == 4);
			if (indexBuffer.data.empty() ||
				indexAccessor.byteOffset + indexBufferView.byteOffset + indexAccessor.count * sizeof(uint32_t) > indexBuffer.data.size()) {
				LOG_FATAL("Index buffer is invalid or out of bounds!");
				return false;
			}

			const uint32_t* buf = reinterpret_cast<const uint32_t*>(&(indexBuffer.data[indexAccessor.byteOffset + indexBufferView.byteOffset]));
			for (size_t index = 0; index < indexAccessor.count / 3; index++)
			{
				MeshFaceData MeshFace;
				for (size_t f = 0; f < 3; f++) {
					uint32_t DataIndex = static_cast<uint32_t>(buf[index + f]);
					MeshFace.vertices[f].position_index = DataIndex;
					MeshFace.vertices[f].texcoord_index = DataIndex;
					MeshFace.vertices[f].normal_index = DataIndex;
				}

				GroupData.Faces.push_back(MeshFace);
			}
		}

		// 位置
		if (primitive.attributes.find("POSITION") != primitive.attributes.end()) {
			const tinygltf::Accessor& positionAccessor = model.accessors[primitive.attributes.at("POSITION")];
			const tinygltf::BufferView& positionView = model.bufferViews[positionAccessor.bufferView];
			// positionAccessor.byteOffset;
			switch (positionAccessor.componentType)
			{
			case TINYGLTF_COMPONENT_TYPE_FLOAT:
			{
				const float* positions = reinterpret_cast<const float*>(&model.buffers[positionView.buffer]
					.data[positionAccessor.byteOffset + positionView.byteOffset]);
				for (size_t i = 0; i < positionAccessor.count; i++) {
					Vec3 vPosition = Vec3(positions[i * 3], positions[i * 3 + 1], positions[i * 3 + 2]);
					Positions.push_back(vPosition);
				}
			} break;
			case TINYGLTF_COMPONENT_TYPE_DOUBLE:
			{
				const double* positions = reinterpret_cast<const double*>(&model.buffers[positionView.buffer]
					.data[positionAccessor.byteOffset + positionView.byteOffset]);
				for (size_t i = 0; i < positionAccessor.count; i++) {
					Vec3 vPosition = Vec3((float)positions[i * 3], (float)positions[i * 3 + 1], (float)positions[i * 3 + 2]);
					Positions.push_back(vPosition);
				}
			} break;
			default:
				return false;
			}

		}

		// 法线
		if (primitive.attributes.find("NORMAL") != primitive.attributes.end()) {
			int normalAccessorIndex = primitive.attributes.at("NORMAL");
			const tinygltf::Accessor& normalAccessor = model.accessors[normalAccessorIndex];
			const tinygltf::BufferView& normalView = model.bufferViews[normalAccessor.bufferView];
			const float* normalData = reinterpret_cast<const float*>(&model.buffers[normalView.buffer]
				.data[normalAccessor.byteOffset + normalView.byteOffset]);

			for (size_t i = 0; i < normalAccessor.count; i++) {
				Normals.push_back(Vec3(normalData[i * 3], normalData[i * 3 + 1], normalData[i * 3 + 2]));
			}
		}

		// 纹理坐标
		if (primitive.attributes.find("TEXCOORD_0") != primitive.attributes.end()) {
			int texCoordAccessorIndex = primitive.attributes.at("TEXCOORD_0");
			const tinygltf::Accessor& texCoordAccessor = model.accessors[texCoordAccessorIndex];
			const tinygltf::BufferView& texCoordView = model.bufferViews[texCoordAccessor.bufferView];
			const float* texCoordData = reinterpret_cast<const float*>(&model.buffers[texCoordView.buffer]
				.data[texCoordAccessor.byteOffset + texCoordView.byteOffset]);

			for (size_t i = 0; i < texCoordAccessor.count; i++) {
				Texcoords.push_back(Vec2(texCoordData[i * 2], texCoordData[i * 2 + 1]));
			}
		}

		// 切线
		if (primitive.attributes.find("TANGENT") != primitive.attributes.end()) {
			int tangentAccessorIndex = primitive.attributes.at("TANGENT");
			const tinygltf::Accessor& tangentAccessor = model.accessors[tangentAccessorIndex];
			const tinygltf::BufferView& tangentView = model.bufferViews[tangentAccessor.bufferView];
			const float* tangentData = reinterpret_cast<const float*>(&model.buffers[tangentView.buffer]
				.data[tangentAccessor.byteOffset + tangentView.byteOffset]);

			for (size_t i = 0; i < tangentAccessor.count; i++) {
				// TODO: 使用切线数据
				Tangents.push_back(Vec3(tangentData[i * 2], tangentData[i * 2 + 1], tangentData[i * 2 + 2]));
			}
		}

		SGeometryConfig NewData;
		NewData.material_name = materialConfigs[primitive.material].name;
		NewData.name = mesh.name;

		ProcessSubobject(Positions, Normals, Texcoords, GroupData.Faces, &NewData);
		out_geometries.push_back(NewData);

		Positions.clear();
		Normals.clear();
		Texcoords.clear();
		GroupData.Faces.clear();
	}

	std::vector<Vec3>().swap(Positions);
	std::vector<Vec3>().swap(Normals);
	std::vector<Vec2>().swap(Texcoords);
	std::vector<Vec3>().swap(Tangents);
	std::vector<uint32_t>().swap(Indices);

	return true;
}
