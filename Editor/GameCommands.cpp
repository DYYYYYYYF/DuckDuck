#include "GameCommands.hpp"
#include <Core/Console.hpp>
#include <Core/Event.hpp>
#include <Core/EngineLogger.hpp>

void GameExit(CommandContext cmd) {
	EngineEvent::Fire(eEventCode::Application_Quit, nullptr, SEventContext());
}

void GameOnCompilerShader(CommandContext cmd) {
	LOG_INFO("Please complete 'GameOnCompilerShader' method.");
	// Reload
	// SEventContext Context = {};
	// EngineEvent::Fire(eEventCode::Reload_Shader_Module, nullptr, Context);
}

void GameCommand::Setup() {
	Console::RegisterCommand("exit", 0, &GameExit);
	Console::RegisterCommand("quit", 0, &GameExit);
	Console::RegisterCommand("compile", 0, &GameOnCompilerShader);
}