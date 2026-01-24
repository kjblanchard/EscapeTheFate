#pragma once
#include <string>
#include <ui/uiImage.hpp>
#include <ui/uiText.hpp>
namespace Etf {

class BattlerUI {
   public:
	BattlerUI(unsigned int battlerNum);
	void UpdateHP(const std::string& hp);
    void MoveCursorInMenu(unsigned int newLocation);

   private:
	UIObject* _menuItems[4];
	UIText* _hpObject;
	UIImage* _finger;
    unsigned int _battlerNum;
    bool _player;
};

}  // namespace Etf
