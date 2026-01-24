#pragma once
#include <string>
#include <ui/uiImage.hpp>
#include <ui/uiProgressBar.hpp>
#include <ui/uiText.hpp>
namespace Etf {

enum class PlayerUIAnimationStates {
	Closed,
	Opening,
	Closing,
	Opened,
};

class BattlerUI {
   public:
	BattlerUI(unsigned int battlerNum);
	void UpdateHP(const std::string& hp);
	void UpdateProgressBar(float percent);
	inline void OpenCommandsMenu() {
		_currentState = PlayerUIAnimationStates::Opening;
		_currentAnimationTime = 0;
	}
	inline void CloseCommandsMenu() {
		_currentState = PlayerUIAnimationStates::Closing;
		_currentAnimationTime = 0;
		_finger->SetVisible(false);
	}
	void MoveCursorInMenu(unsigned int newLocation);
	void UpdateAnimations();

   private:
   private:
	UIObject* _commandMenu;
	UIObject* _menuItems[4];
	UIText* _hpObject;
	UIImage* _finger;
	UIProgressBar* _progressBar;
	unsigned int _battlerNum;
	bool _player;
	float _currentAnimationTime = 0;
	float _menuBoxStartX, _menuBoxStartY;
	PlayerUIAnimationStates _currentState = PlayerUIAnimationStates::Closed;
};

}  // namespace Etf
