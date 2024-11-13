#include "Sound.hpp"
#include "sndfile.h"
#include "Core/EngineLogger.hpp"

Sound::Sound() {
	Buffer = NULL;
	Source = NULL;
}

Sound::~Sound() {
	Stop();
	alDeleteSources(1, &Source);
	alDeleteBuffers(1, &Buffer);
}

bool Sound::Load(const std::string& filename) {
	// 使用 libsndfile 或其他库加载音频文件数据
	SNDFILE* file;
	SF_INFO sfInfo;
	file = sf_open(filename.c_str(), SFM_READ, &sfInfo);
	if (!file) {
		LOG_ERROR("Failed to load sound file: ", filename);
		return false;
	}

	// 获取音频数据
	short* Samples = new short[sfInfo.frames * sfInfo.channels];
	sf_read_short(file, Samples, sfInfo.frames * sfInfo.channels);
	sf_close(file);

	// 创建 OpenAL 音频缓冲区
	alGenBuffers(1, &Buffer);
	alBufferData(Buffer, AL_FORMAT_MONO16, Samples, sfInfo.frames * sfInfo.channels * sizeof(short), sfInfo.samplerate);
	delete[] Samples;

	// 创建音频源
	alGenSources(1, &Source);
	alSourcei(Source, AL_BUFFER, Buffer);

	return true;
}

void Sound::Play() {
	alSourcePlay(Source);
}

void Sound::Stop() {
	alSourceStop(Source);
}