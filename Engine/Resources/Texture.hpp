#pragma once

#include "Math/MathTypes.hpp"

#define TEXTURE_NAME_MAX_LENGTH 512

enum TextureUsage {
	eTexture_Usage_Unknown = 0x00,
	eTexture_Usage_Map_Diffuse = 0x01,
	eTexture_Usage_Map_Specular = 0x02,
	eTexture_Usage_Map_Normal = 0x03,
	eTexture_Usage_Map_Cubemap = 0x04
};

enum TextureFilter {
	eTexture_Filter_Mode_Nearest = 0x0,
	eTexture_Filter_Mode_Linear = 0x1
};

enum TextureRepeat{
	eTexture_Repeat_Repeat = 0x0,
	eTexture_Repeat_Minrrored_Repeat = 0x1,
	eTexture_Repeat_Clamp_To_Edge = 0x2,
	eTexture_Repeat_Clamp_To_Border = 0x3,
};

enum TextureFlagBits{
	eTexture_Flag_Has_Transparency = 0x1,
	eTexture_Flag_Is_Writeable = 0x2,
	eTexture_Flag_Is_Wrapped = 0x4,
	eTexture_Flag_Depth = 0x8
};
typedef unsigned char TextureFlag;

enum TextureType {
	eTexture_Type_2D,
	eTexture_Type_Cube
};

struct TextureMap {
	class Texture* texture = nullptr;
	TextureUsage usage = TextureUsage::eTexture_Usage_Unknown;
	TextureFilter filter_minify = TextureFilter::eTexture_Filter_Mode_Linear;
	TextureFilter filter_magnify = TextureFilter::eTexture_Filter_Mode_Linear;
	TextureRepeat repeat_u = TextureRepeat::eTexture_Repeat_Repeat;
	TextureRepeat repeat_v = TextureRepeat::eTexture_Repeat_Repeat;
	TextureRepeat repeat_w = TextureRepeat::eTexture_Repeat_Repeat;
	void* internal_data = nullptr;
};

class DAPI Texture {
public:
	uint32_t Id = INVALID_ID;
	TextureType Type = TextureType::eTexture_Type_2D;
	uint32_t Width = 100;
	uint32_t Height = 100;

	int ChannelCount = 4;
	/** TextureFlag */
	TextureFlag Flags = 0;

	uint32_t Generation = INVALID_ID;
	char Name[TEXTURE_NAME_MAX_LENGTH] = "";
	void* InternalData = nullptr;
};
