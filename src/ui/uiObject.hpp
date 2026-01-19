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
	inline float X() {return _location.x;}
	inline float Y() {return _location.y;}
	inline void SetX(float x){_location.x = x;}
	inline void SetY(float y){_location.y = y;}
	// Gets the position on the screen currently, currently recursive call to root so can be expensive.
	RectangleF GetAbsolutePosition();
	//Sets the position based on absolute position, can be expensive as it calls getAbsolute position on itself
	void SetAbsolutePosition(int x, int y);
	// What to do when this is dirty, called before
	virtual void OnDirty() {}
	// Calls dirty on all children.
	void Dirty();
	// How this Object draws itself
	virtual void OnDraw(float offsetX, float offsetY) {}
	// Calls ondraw and then draws all children, left virtual for hlg/vlg
	virtual void Draw(float offsetX, float offsetY);
	virtual ~UIObject() = default;
	// Recursive search, can be expensive if you search high
	UIObject* GetChildByName(const std::string& name);
	void AddChild(UIObject* newChild);
	// Do we have a child object with this name
	bool HasChildOfName(const std::string& name);
	// Do we have a child object with this name, or do we have a child that has it
	inline void SetVisible(bool isVisible) { _visible = isVisible; }
	bool HasChildOfNameInAllChildren(const std::string& name);
	void DestroyChildByName(const std::string& name, bool force = false);
	void DestroyChildByName(const std::vector<std::string> names, bool force = false);
	void DestroyChildIfNotName(const std::string& name, bool force = false);
	void DestroyChildIfNotName(const std::vector<std::string> names, bool force = false);

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
