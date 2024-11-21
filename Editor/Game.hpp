#pragma once

#include "GameConsole.hpp"
#include <Defines.hpp>
#include <IGame.hpp>
#include <Math/MathTypes.hpp>
#include <Core/CPython.hpp>
#include <Core/Keymap.hpp>

class Camera;

class GameInstance : public IGame {
public:
	GameInstance() : WorldCamera(nullptr), Width(1920), Height(1080), ConsoleKeymap(nullptr){}
	virtual ~GameInstance() {};

public:
	virtual bool Boot(IRenderer* renderer) override;
	virtual void Shutdown() override;
	virtual bool Initialize() override;
	virtual bool Update(float delta_time) override;
	virtual bool Render(struct SRenderPacket* packet, float delta_time) override;
	virtual void OnResize(unsigned int width, unsigned int height) override;

public:
	short Width, Height;
	Camera* WorldCamera;
	Frustum CameraFrustum;

	// TODO: temp
	Skybox SB;
	Keymap* ConsoleKeymap;
	DebugConsole* GameConsole = nullptr;

	std::vector<Mesh> Meshes;
	Mesh* CarMesh = nullptr;
	Mesh* SponzaMesh = nullptr;
	Mesh* DragonMesh = nullptr;
	Mesh* BunnyMesh = nullptr;

	std::vector<Mesh> UIMeshes;
	UIText TestText;
	UIText TestSysText;

	uint32_t HoveredObjectID = INVALID_ID;
	CPythonModule TestPython;
	// TODO: end temp

};
