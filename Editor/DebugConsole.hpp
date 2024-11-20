#include <Core/Event.hpp>
#include <Resources/UIText.hpp>

class IRenderer;

class DebugConsole{
public:
	DebugConsole();
	DebugConsole(IRenderer* renderer);
	virtual ~DebugConsole();

public:
	bool Load();
	void Update();

	UIText* GetText();
	UIText* GetEntryText();

	void MoveUp();
	void MoveDown();
	void MoveToTop();
	void MoveToBottom();

	bool IsVisible() const { return Visible; }
	void SetVisible(bool visiblable) { Visible = visiblable; }

	bool OnKey(eEventCode code, void* sender, void* listener_inst, SEventContext context);

private:
	bool Write(Log::Logger::Level level, const std::string& msg);

private:
	size_t DisplayLineCount;
	size_t LineOffset;
	std::vector<std::string> Lines;

	bool Dirty;
	bool Visible;

	UIText* TextControl;	// Log text.
	UIText* EntryControl;	// Command text.

	IRenderer* Renderer;
};