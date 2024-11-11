#pragma once
#include "AudioManager.hpp"

#include <AL/al.h>
#include <AL/alc.h>

class AudioEngine {
public:
	DAPI AudioEngine();
	DAPI virtual ~AudioEngine();

public:
	DAPI bool Initalize();
	DAPI void Update();
	DAPI void Shutdown();

public:
	DAPI AudioManager* GetManager() { return Manager; }

private:
	ALCdevice* aDevice;
	ALCcontext* aContext;
	AudioManager* Manager;
};
