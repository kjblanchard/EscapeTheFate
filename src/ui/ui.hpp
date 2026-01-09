#pragma once
#include <memory>
#include <ui/uiObject.hpp>
namespace Etf {
class UI {
   public:
	static void LoadUIFromFile(const std::string& filename);
	static void DrawUI();
	static void DestroyUI();
	static std::unique_ptr<UIObject> RootUIObject;
};
}  // namespace Etf
