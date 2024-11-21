#include "GameCommands.hpp"
#include <Core/Console.hpp>
#include <Core/Event.hpp>

void GameExit(CommandContext cmd) {
	EngineEvent::Fire(eEventCode::Application_Quit, nullptr, SEventContext());
}

void GameCommand::Setup() {
	Console::RegisterCommand("exit", 0, &GameExit);
	Console::RegisterCommand("quit", 0, &GameExit);
}