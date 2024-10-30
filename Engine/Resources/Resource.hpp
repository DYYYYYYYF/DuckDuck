#pragma once

#include "Math/MathTypes.hpp"

/** @brief A magic number indicating the file as engine file. */
#define RESOURCES_MAGIC 0xdddddddd

enum ResourceType {
	eResource_type_Text,
	eResource_type_Binary,
	eResource_type_Image,
	eResource_type_Material,
	eResource_type_Static_Mesh,
	eResource_Type_Shader,
	eResource_Type_Bitmap_Font,
	eResource_Type_System_Font,
	eResource_type_Custom,
};

struct ResourceHeader {
	uint32_t magicNumber;
	unsigned char resourceType;
	unsigned char version;
	unsigned short reserved;
};

class Resource {
public:
	uint32_t LoaderID = INVALID_ID;
	char* Name = nullptr;
	char* FullPath = nullptr;
	size_t DataSize = 0;
	size_t DataCount = 0;
	void* Data = nullptr;
};

struct ImageResourceData {
	unsigned char channel_count = 4;
	uint32_t width = 1920;
	uint32_t height = 1080;
	unsigned char* pixels = nullptr;
};

struct ImageResourceParams {
	bool flip_y = false;
};