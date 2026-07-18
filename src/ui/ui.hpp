#pragma once
#include <memory>
#include <ui/uiObject.hpp>
namespace Etf {
namespace UI {
	 void LoadUIFromFile(const std::string& filename);
	 void DrawUI();
	 void DestroyUI();
	 UIObject* GetRootUIObject();
};
}  // namespace Etf
