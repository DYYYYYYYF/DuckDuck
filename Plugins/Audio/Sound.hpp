#pragma once
#include <AL/al.h>
#include <AL/alc.h>
#include <string>

#ifdef EXPORT
// Export
#ifdef _MSC_VER
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT __attribute__((visibility("default")))
#endif

// Import
#else
#ifdef _MSC_VER
#define DLLEXPORT __declspec(dllimport)
#else
#define DLLEXPORT
#endif

#endif	// #ifdef DEXPORT

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