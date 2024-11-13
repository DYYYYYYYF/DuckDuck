#ifdef ENABLE_AUDIO
#include <AudioEngine.hpp>
#include <thread>

bool UnitTestAudio() {
	AudioEngine Engine;
	if (Engine.Initalize()) {
		Engine.Update();

		// ���ز�����
		LOG_INFO("Start play sound %s", "Footsteps.wav");
		Engine.GetManager()->LoadSound("../Assets/Media/Audio/sample1.wav");
		Engine.GetManager()->PlaySound("../Assets/Media/Audio/sample1.wav");

		// �ȴ�����
		std::this_thread::sleep_for(std::chrono::seconds(10));
		Engine.Shutdown();
	}
	return true;
}

#else 
#include <Core/EngineLogger.hpp>

bool UnitTestAudio() {
	LOG_INFO("Not enable plugins audio. Skip audio unit test. If you want to do audio test, set ENABLE_PLUGINS_AUDIO ON on CMakeLists.");
	return true;
}

#endif
