#pragma once
#include <ui/uiObject.hpp>

namespace Etf {

struct UIScrollListArgs {
	std::string Name;
	RectangleF Rect;
	int Priority = 0;
	bool Visible = true;
};

class UIScrollList : public UIObject {
   public:
	UIScrollList(UIScrollListArgs& args);
	void Draw(float offsetX, float offsetY) override;
	void Scroll(int deltaPixels);
	void SetScrollOffset(int offset);
	inline int GetScrollOffset() const { return _scrollOffset; }
	void SetContentHeight(int height);

   private:
	int _scrollOffset = 0;
	int _contentHeight = 0;
};

}  // namespace Etf
