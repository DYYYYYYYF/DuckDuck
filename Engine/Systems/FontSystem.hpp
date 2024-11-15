#pragma once

#include "Math/MathTypes.hpp"
#include "Containers/TString.hpp"
#include "Resources/ResourceTypes.hpp"
#include "Renderer/RendererTypes.hpp"

struct BitmapFontLookup;
struct SystemFontLookup;

struct SystemFontConfig {
public:
	SystemFontConfig() {
		name = nullptr;
		resourceName = nullptr;
		defaultSize = 0;
	}
	
	SystemFontConfig(const SystemFontConfig& s) {
		name = s.name;
		defaultSize = s.defaultSize;
		resourceName = s.resourceName;
	}

	const char* name;
	unsigned short defaultSize;
	const char* resourceName;
};

struct BitmapFontConfig {
public:
	BitmapFontConfig() {
		name = nullptr;
		resourceName = nullptr;
		size = 0;
	}
	
	BitmapFontConfig(const BitmapFontConfig& b) {
		name = b.name;
		size = b.size;
		resourceName = b.resourceName;
	}

	const char* name;
	unsigned short size;
	const char* resourceName;
};

struct FontSystemConfig {
	unsigned char defaultSystemFontCount = 0;
	SystemFontConfig* systemFontConfigs = nullptr;
	unsigned char defaultBitmapFontCount = 0;
	BitmapFontConfig* bitmapFontConfigs = nullptr;
	unsigned char maxSystemFontCount = 10;
	unsigned char maxBitmapFontCount = 10;
	bool autoRelease = false;
};

class FontSystem {
public:
	static bool Initialize(IRenderer* renderer, FontSystemConfig* config);
	static void Shutdown();

	static bool LoadSystemFont(SystemFontConfig* config);
	static bool LoadBitmapFont(BitmapFontConfig* config);

	static bool Acquire(const char* fontName, unsigned short fontSize, class UIText* text);
	static bool Release(class UIText* text);

	static bool VerifyAtlas(struct FontData* data, const char* text);


private:
	static bool SetupFontData(FontData* font);
	static void CleanupFontData(FontData* font);

	// System fonts.
	static bool CreateSystemFontVariant(SystemFontLookup* lookup, unsigned short size, const char* fontName, FontData* outVariant);
	static bool RebuildSystemFontVariantAtlas(SystemFontLookup* lookip, FontData* variant);
	static bool VerifySystemFontSizeVariant(SystemFontLookup* lookup, FontData* variant, const char* text);

private:
	static FontSystemConfig Config;
	static HashTable BitFontLookup;
	static HashTable SysFontLookup;
	static BitmapFontLookup* BitmapFonts;
	static SystemFontLookup* SystemFonts;
	static void* BitmapHashTableBlock;
	static void* SystemHashTableBlock;

	static IRenderer* Renderer;
	static bool Initilized;

};
