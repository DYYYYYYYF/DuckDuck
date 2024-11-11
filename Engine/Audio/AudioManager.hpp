#pragma once
#include <Al/al.h>
#include "Sound.hpp"
#include <string>
#include <unordered_map>

class Sound;

class AudioManager {
public:
	AudioManager();
	virtual ~AudioManager();

public:
	bool Initialize();
	void Shutdown();

	DAPI bool LoadSound(const std::string& filename);
	DAPI void PlaySound(const std::string& filename);

private:
	std::unordered_map<std::string, Sound*> Sounds;
};