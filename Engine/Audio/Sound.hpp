#pragma once
#include "Defines.hpp"

#include <AL/al.h>
#include <AL/alc.h>
#include <string>

class Sound {
public:
	Sound();
	virtual ~Sound();

public:
	bool Load(const std::string& filename);
	void Play();
	void Stop();

private:
	ALuint Buffer;
	ALuint Source;
};