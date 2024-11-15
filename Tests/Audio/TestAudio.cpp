#ifdef ENABLE_AUDIO
#include <AudioEngine.hpp>
#include <thread>

bool UnitTestAudio() {
	AudioEngine Engine;
	if (Engine.Initalize()) {
		Engine.Update();
        
#ifdef _WIN32
        std::string AssetPath("../Assets/Media/Audio/sample1.wav");
#else
        std::string AssetPath("../../Assets/Media/Audio/sample1.wav");
#endif

		// ���ز�����
		LOG_INFO("Start play sound %s", "Footsteps.wav");
		Engine.GetManager()->LoadSound(AssetPath);
		Engine.GetManager()->PlaySound(AssetPath);

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
