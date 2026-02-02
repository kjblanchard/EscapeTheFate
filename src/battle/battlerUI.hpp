#pragma once
#include <string>
#include <ui/uiImage.hpp>
#include <ui/uiProgressBar.hpp>
#include <ui/uiText.hpp>
namespace Etf {
class Battler;

enum class PlayerUIAnimationStates {
	Closed,
	Opening,
	Closing,
	Opened,
};

class BattlerUI {
   public:
	BattlerUI(unsigned int battlerNum);
	~BattlerUI();
	void UpdateHP(const std::string& hp);
	void UpdateProgressBar(float percent);
	inline void OpenCommandsMenu() {
		_commandMenu->SetVisible(true);
		_currentState = PlayerUIAnimationStates::Opening;
		_currentAnimationTime = 0;
	}
	inline void CloseCommandsMenu() {
		_currentState = PlayerUIAnimationStates::Closing;
		_currentAnimationTime = 0;
		_finger->SetVisible(false);
	}
	void StartTargetSelection();
	void MoveFingerToBattlerLocation(Battler* battler);
	void CloseTargetSelection();
	void MoveCursorInMenu(unsigned int newLocation);
	void UpdateAnimations();
	// Used when battle is over currently
	static void ClosePlayerInfoBox();

   private:
	UIObject* _commandMenu;
	UIObject* _menuItems[4];
	UIText* _hpObject;
	UIImage* _finger;
	UIImage* _targetSelectionFinger;
	UIProgressBar* _progressBar;
	unsigned int _battlerNum;
	bool _player;
	float _currentAnimationTime = 0;
	float _menuBoxStartX, _menuBoxStartY;
	PlayerUIAnimationStates _currentState = PlayerUIAnimationStates::Closed;
};

}  // namespace Etf
