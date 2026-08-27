#pragma once
#include <Supergoon/Primitives/rectangle.h>

#include <functional>
#include <vector>

namespace Etf {
class UIObject;

struct UIButtonDef {
	UIObject* target;
	int index;
};

class UIButtonGroup {
   public:
	using FocusCallback = std::function<void(int)>;
	using ActivateCallback = std::function<void(int)>;

	UIButtonGroup(FocusCallback onFocus, ActivateCallback onActivate);
	void AddButton(UIObject* target, int logicalIndex);
	void ClearButtons();
	void SetFocusedIndex(int index);
	int FocusedIndex() const { return _focusedIndex; }
	void Update(float pointerX, float pointerY, bool justClicked);

   private:
	std::vector<UIButtonDef> _buttons;
	int _focusedIndex = -1;
	FocusCallback _onFocus;
	ActivateCallback _onActivate;
};

class UIButtonGroupRect {
   public:
	using FocusCallback = std::function<void(int)>;
	using ActivateCallback = std::function<void(int)>;

	UIButtonGroupRect(FocusCallback onFocus, ActivateCallback onActivate);
	void AddButton(RectangleF rect, int logicalIndex);
	void ClearButtons();
	void SetFocusedIndex(int index);
	int FocusedIndex() const { return _focusedIndex; }
	void Update(float pointerX, float pointerY, bool justClicked);

   private:
	struct RectButtonDef {
		RectangleF rect;
		int index;
	};
	std::vector<RectButtonDef> _buttons;
	int _focusedIndex = -1;
	FocusCallback _onFocus;
	ActivateCallback _onActivate;
};

namespace PointerInputSystem {
void Start();
void Update();
void PushGroup(UIButtonGroup* group);
void PushGroupRect(UIButtonGroupRect* group);
void PopGroup(UIButtonGroup* group);
void PopGroupRect(UIButtonGroupRect* group);
}  // namespace PointerInputSystem
}  // namespace Etf
