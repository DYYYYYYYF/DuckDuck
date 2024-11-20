#include "DebugConsole.hpp"
#include <Core/Utils.hpp>
#include <Core/Console.hpp>
#include <Renderer/RendererFrontend.hpp>

bool DebugConsole::Write(Log::Logger::Level level, const std::string& msg) {
	std::vector<std::string> SplitMessage = Utils::StringSplit(msg, '\n', true, false);
	for (size_t i = 0; i < SplitMessage.size(); ++i) {
		Lines.push_back(SplitMessage[i]);
	}

	Dirty = true;
	return true;
}

bool DebugConsole::OnKey(eEventCode code, void* sender, void* listener_inst, SEventContext context) {
	

	return true;
}

DebugConsole::DebugConsole(){
	DisplayLineCount = 10;
	LineOffset = 0;
	Visible = false;
	Renderer = nullptr;
	TextControl = nullptr;
	EntryControl = nullptr;

	Console::RegisterConsumer(std::bind(&DebugConsole::Write, this, std::placeholders::_1, std::placeholders::_2));
}

DebugConsole::DebugConsole(IRenderer* renderer) {
	DisplayLineCount = 10;
	LineOffset = 0;
	Visible = false;
	Renderer = renderer;
	TextControl = nullptr;
	EntryControl = nullptr;

	Console::RegisterConsumer(std::bind(&DebugConsole::Write, this, std::placeholders::_1, std::placeholders::_2));
}

DebugConsole::~DebugConsole() {
	if (TextControl) {
		TextControl->Destroy();
		DeleteObject(TextControl);
		TextControl = nullptr;
	}

	if (EntryControl) {
		EntryControl->Destroy();
		DeleteObject(EntryControl);
		EntryControl = nullptr;
	}

	Console::UnregisterConsumer(std::bind(&DebugConsole::Write, this, std::placeholders::_1, std::placeholders::_2));
}

bool DebugConsole::Load() {
	// Create UI text control for rendering.
	TextControl = NewObject<UIText>();
	if (!TextControl->Create(Renderer, UITextType::eUI_Text_Type_system, "Noto Sans CJK JP", 26, "Test text control.")) {
		LOG_FATAL("Unable to create text control for debug console.");
		return false;
	}

	TextControl->SetPosition(Vec3(0.7f * Renderer->GetWidth(), 100, 0));

	// Create another ui text control for rendering typed text.
	EntryControl = NewObject<UIText>();
	if (!EntryControl->Create(Renderer, UITextType::eUI_Text_Type_system, "Noto Sans CJK JP", 26, "Test entry control.")) {
		LOG_FATAL("Unable to create entry control for debug console.");
		return false;
	}

	EntryControl->SetPosition(Vec3(0.7f * Renderer->GetWidth(), 100 + (31.0f * DisplayLineCount), 0.0f));

	EngineEvent::Register(eEventCode::Key_Pressed, nullptr,
		std::bind(
			&DebugConsole::OnKey, this,
			std::placeholders::_1,
			std::placeholders::_2,
			std::placeholders::_3,
			std::placeholders::_4
		)
	);
	EngineEvent::Register(eEventCode::Key_Released, nullptr,
		std::bind(
			&DebugConsole::OnKey, this,
			std::placeholders::_1,
			std::placeholders::_2,
			std::placeholders::_3,
			std::placeholders::_4
		)
	);

	return true;
}

void DebugConsole::Update() {
	if (!Dirty) {
		return;
	}

	size_t LineCount = Lines.size();
	size_t MaxLines = DMIN(DisplayLineCount, DMAX(LineCount, DisplayLineCount));

	// Calculate the min line first, taking into account the line offset as well.
	size_t MinLine = DMAX(LineCount - MaxLines - LineOffset, 0);
	size_t MaxLine = MinLine + MaxLines - 1;

	std::string Buffer = "";
	for (size_t i = MinLine; i < MaxLine; ++i) {
		// TODO: insert colour codes for the message type.

		// Copy line
		Buffer += Lines[i];
		// New line
		Buffer += "\n";
	}

	// Once the string is built, set the text.
	TextControl->SetText(Buffer.c_str());
	Dirty = false;
}

UIText* DebugConsole::GetText() {
	return TextControl;
}

UIText* DebugConsole::GetEntryText() {
	return EntryControl;
}

void DebugConsole::MoveUp() {
	Dirty = true;
	size_t LineCount = Lines.size();
	// Don't bother with trying an offset, just reset and boot out.
	if (LineCount <= DisplayLineCount) {
		LineOffset = 0;
		return;
	}

	LineOffset++;
	LineOffset = DMIN(LineOffset, LineCount - DisplayLineCount);
}

void DebugConsole::MoveDown() {
	Dirty = true;
	size_t LineCount = Lines.size();
	// Don't bother with trying an offset, just reset and boot out.
	if (LineCount <= DisplayLineCount) {
		LineOffset = 0;
		return;
	}

	LineOffset--;
	LineOffset = DMAX(LineOffset, 0);
}

void DebugConsole::MoveToTop() {
	Dirty = true;
	size_t LineCount = Lines.size();
	// Don't bother with trying an offset, just reset and boot out.
	if (LineCount <= DisplayLineCount) {
		LineOffset = 0;
		return;
	}

	LineOffset = LineCount - DisplayLineCount;
}

void DebugConsole::MoveToBottom() {
	Dirty = true;
	LineOffset = 0;
}
