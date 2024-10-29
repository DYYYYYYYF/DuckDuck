#pragma once

#include "Math/MathTypes.hpp"
#include "Resources/Texture.hpp"

#define DEFAULT_MATERIAL_NAME "Builtin.Material.Default"
#define VULKAN_MAX_MATERIAL_COUNT 1024

class Texture;

struct SMaterialConfig {
	std::string name;
	std::string shader_name;
	bool auto_release;
	Vec4 diffuse_color;
	float shininess;
	char diffuse_map_name[TEXTURE_NAME_MAX_LENGTH] = "";
	char specular_map_name[TEXTURE_NAME_MAX_LENGTH] = "";
	char normal_map_name[TEXTURE_NAME_MAX_LENGTH] = "";

	// PBR
	float Metallic;
	float Roughness;
};

class Material {
public:
	Material() {
		Id = INVALID_ID;
		Generation = INVALID_ID;
		InternalId = INVALID_ID;
		DiffuseColor = Vec4(1.0f);
		Shininess = 32.0f;
		ShaderID = INVALID_ID;
		RenderFrameNumer = 0;
		Metallic = 1.0f;
		Roughness = 32.0f;
	}

public:
	uint32_t Id;
	uint32_t Generation;
	uint32_t InternalId;
	std::string Name;
	Vec4 DiffuseColor;
	TextureMap DiffuseMap;
	TextureMap SpecularMap;
	TextureMap NormalMap;
	float Shininess;

	uint32_t ShaderID;
	uint32_t RenderFrameNumer;

	// PBR
	float Metallic;
	float Roughness;
};
