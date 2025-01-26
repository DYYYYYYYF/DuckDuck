﻿#pragma once

#include "Math/MathTypes.hpp"
#include "Math/Transform.hpp"
#include "Renderer/RendererTypes.hpp"
#include "Renderer/Vulkan/VulkanBuffer.hpp"
#include "Renderer/Interface/IRenderbuffer.hpp"

class IFontDataBase;

enum UITextType {
	eUI_Text_Type_Bitmap,
	eUI_Text_Type_system
};

class UIText {
public:
	DAPI bool Create(class IRenderer* renderer, UITextType type, const std::string& fontName, unsigned short fontSize, const char* textContent);
	DAPI void Destroy();

	DAPI void SetPosition(Vector3 position);
	DAPI void SetText(const char* text);

	void Draw();

public:
	DAPI std::string GetName() const { return Name; }
	DAPI void SetName(const std::string& n) { Name = n; }

	DAPI Vector4 GetColor() const { return Color; }
	DAPI void SetColor(Vector4 col) { Color = col; }

private:
	void RegenerateGeometry();

public:
	uint32_t UniqueID = INVALID_ID;
	IRenderer* Renderer = nullptr;
	UITextType Type = UITextType::eUI_Text_Type_Bitmap;
	IFontDataBase* Data = nullptr;
	IRenderbuffer* VertexBuffer = nullptr;
	IRenderbuffer* IndexBuffer = nullptr;
	char* Text = nullptr;
	Transform Trans;
	uint32_t InstanceID = INVALID_ID;
	size_t RenderFrameNumber = 0;

private:
	std::string Name;
	Vector4 Color = Vector4(1.0f);

};