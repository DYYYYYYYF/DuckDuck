#include "Console.hpp"
#include "Core/utils.hpp"

const uint32_t MAX_CONSUMER_COUNT = 10;

std::vector<Console::Consumer> Console::RegisteredConsumers;
std::vector<Console::Command> Console::RegisteredCommands;

void Console::Initialize() {

}

void Console::Shutdown() {

}

void Console::RegisterConsumer(PFN_ConsoleWrite callback) {
	Consumer NewConsumer;
	NewConsumer.Instance = nullptr;
	NewConsumer.Callback = std::move(callback);
	RegisteredConsumers.push_back(NewConsumer);
}

void Console::UnregisterConsumer(PFN_ConsoleWrite callback) {
	for (size_t i = 0; i < RegisteredConsumers.size(); ++i) {
		Consumer& consumer = RegisteredConsumers[i];
		if (consumer.Callback.target<bool(Log::Logger::Level, const std::string&)>() ==
			callback.target<bool(Log::Logger::Level, const std::string&)>()) {
			RegisteredConsumers.erase(RegisteredConsumers.begin() + i);
			continue;
		}
	}
}

void Console::WriteLine(Log::Logger::Level level, const std::string& msg) {
	// Notify each consumer that a line has been added.
	for (unsigned char i = 0; i < RegisteredConsumers.size(); ++i) {
		RegisteredConsumers[i].Callback(level, msg);
	}
}

bool Console::RegisterCommand(const std::string& cmd, unsigned char arg_count, PFN_ConsoleCommand func) {
	// Make sure it doesn't already exist.
	uint32_t CommandCount = (uint32_t)RegisteredCommands.size();
	for (uint32_t i = 0; i < CommandCount; ++i) {
		if (RegisteredCommands[i].Name.compare(cmd) == 0) {
			LOG_ERROR("Command already registered: %s.", cmd.c_str());
			return false;
		}
	}

	Command NewCommand;
	NewCommand.ArgCount = arg_count;
	NewCommand.Func = func;
	NewCommand.Name = cmd;
	RegisteredCommands.push_back(NewCommand);

	return true;
}

bool Console::ExecuteCommand(const std::string& cmd) {
	if (cmd.length() == 0) {
		return false;
	}

	// TODO: If strings are ever used as arguments, this will split improperly.
	std::vector<std::string> Parts = Utils::StringSplit(cmd, ' ', true, false);
	if (Parts.size() < 1){
		Parts.clear();
		std::vector<std::string>().swap(Parts);
		return false;
	}

	// Write the line back out to the console for reference.
	std::string Temp = "-->" + cmd;
	WriteLine(Log::Logger::INFO, Temp);

	// Strings are slow. But it's a console. It doesn't need to be lightning fast.
	// TODO: Complete


	return true;
}
