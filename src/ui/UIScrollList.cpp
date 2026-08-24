#ifndef __EMSCRIPTEN__
#include <glad/glad.h>
#else
#include <GLES3/gl3.h>
#endif

#include <algorithm>
#include <ui/UIScrollList.hpp>

using namespace Etf;

UIScrollList::UIScrollList(UIScrollListArgs& args) {
	_location = args.Rect;
	_name = args.Name;
	_priority = args.Priority;
	_visible = args.Visible;
}

void UIScrollList::SetContentHeight(int height) {
	_contentHeight = height;
}

void UIScrollList::Scroll(int deltaPixels) {
	SetScrollOffset(_scrollOffset + deltaPixels);
}

void UIScrollList::SetScrollOffset(int offset) {
	int maxScroll = std::max(0, _contentHeight - static_cast<int>(_location.h));
	_scrollOffset = std::max(0, std::min(offset, maxScroll));
}

void UIScrollList::Draw(float offsetX, float offsetY) {
	if (!_visible) return;

	float absX = offsetX + _location.x;
	float absY = offsetY + _location.y;
	int clipX = static_cast<int>(absX);
	int clipY = static_cast<int>(absY);
	int clipW = static_cast<int>(_location.w);
	int clipH = static_cast<int>(_location.h);

	glEnable(GL_SCISSOR_TEST);
	glScissor(clipX, clipY, clipW, clipH);

	for (auto& child : _children) {
		child->Draw(absX, absY - static_cast<float>(_scrollOffset));
	}

	glDisable(GL_SCISSOR_TEST);
}
