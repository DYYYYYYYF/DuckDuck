#pragma once
#include "AudioManager.hpp"

#include <AL/al.h>
#include <AL/alc.h>

class AudioEngine {
public:
	DLLEXPORT AudioEngine();
	DLLEXPORT virtual ~AudioEngine();

public:
	DLLEXPORT bool Initalize();
	DLLEXPORT void Update();
	DLLEXPORT void Shutdown();

public:
	DLLEXPORT AudioManager* GetManager() { return Manager; }

private:
	ALCdevice* aDevice;
	ALCcontext* aContext;
	AudioManager* Manager;
};
