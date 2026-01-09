#pragma once
#include <Supergoon/Primitives/rectangle.h>

#include <memory>
#include <string>
#include <vector>
namespace Etf {
struct UIObjectArgs {
	RectangleF Rect;
	bool Visible = false;
	bool DoNotDestroy = false;
	bool DebugBox = false;
	std::string Name;
	int Priority = 0;
};

class UIObject {
   public:
	UIObject(UIObjectArgs args);
	UIObject() = default;
	// Gets the position on the screen currently, currently recursive call to root so can be expensive.
	RectangleF GetAbsolutePosition();
	// What to do when this is dirty, called before
	virtual void OnDirty() {}
	// Calls dirty on all children.
	void Dirty();
	// How this Object draws itself
	virtual void OnDraw(float offsetX, float offsetY) {}
	// Calls ondraw and then draws all children, left virtual for hlg/vlg
	void Draw(float offsetX, float offsetY);
	virtual ~UIObject() = default;
	// Recursive search, can be expensive if you search high
	UIObject* GetChildByName(const std::string& name);
	void AddChild(UIObject* newChild);
	// Do we have a child object with this name
	bool HasChildOfName(const std::string& name);
	// Do we have a child object with this name, or do we have a child that has it
	bool HasChildOfNameInAllChildren(const std::string& name);

   protected:
	// Will call OnDirty when dirty.
	bool _dirty = true;
	bool _doNotDestroy = false;
	// will not draw self or children if not visible;
	bool _visible = true;
	// Used when drawing, higher priority is drawn on top of lower
	unsigned int _priority = 0;
	std::string _name = "root";
	std::vector<std::unique_ptr<UIObject>> _children;
	UIObject* _parent = nullptr;
	// local location; Offset to parent and size
	RectangleF _location = {0, 0, 0, 0};
	bool _debugBox = false;
};
}  // namespace Etf
