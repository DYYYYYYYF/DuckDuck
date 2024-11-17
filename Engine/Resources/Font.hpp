#pragma once

#include "Resource.hpp"
#include <vector>
#include <string>

enum FontType {
	eFont_Type_Bitmap,
	eFont_Type_System
};

struct FontGlyph {
	int codePoint;
	unsigned short x;
	unsigned short y;
	unsigned short width;
	unsigned short height;
	short offsetX;
	short offsetY;
	short advanceX;
	unsigned char pageID;
};

struct FontKerning {
	int codePoint0;
	int codePoint1;
	short amount;
};

class FontData {
public:
	FontType type = FontType::eFont_Type_Bitmap;
	std::string face;
	unsigned int size = 0;
	int lineHeight = -1;
	int baseLine = -1;
	int atlasSizeX = 1024;
	int atlasSizeY = 1024;
	struct TextureMap atlas;
	unsigned int glyphCount = 0;
	FontGlyph* glyphs = nullptr;
	unsigned int kerningCount = 0;
	FontKerning* kernings = nullptr;
	float tabXAdvance = 0.0f;
	unsigned int internalDataSize = 0;
	void* internalData = nullptr;
};

struct BitmapFontPage {
	char id = INVALID_ID_U8;
	std::string file;
};

struct BitmapFontResourceData {
	FontData data;
	unsigned int pageCount = 0;
	BitmapFontPage* Pages = nullptr;
};

struct SystemFontFace {
	std::string name;
};

struct SystemFontResourceData {
	std::vector<SystemFontFace> fonts;
	size_t binarySize = 0;
	void* fontBinary = nullptr;
};