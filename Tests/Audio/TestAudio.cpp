#include <Audio/AudioEngine.hpp>
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
		std::this_thread::sleep_for(std::chrono::seconds(60));
		Engine.Shutdown();
	}
	return true;
}
