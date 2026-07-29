#pragma once
#include <string>
#include <ui/uiAnimation.hpp>
#include <ui/uiImage.hpp>
#include <ui/uiNineSlice.hpp>
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
		CloseMagicMenu();
	}
	inline void StartATBIdleAnim() {
		if (_progressBarAnim) _progressBarAnim->GetAnimator().StartAnimation("idle");
	}
	inline void StartATBTurnAnim() {
		if (_progressBarAnim) _progressBarAnim->GetAnimator().StartAnimation("turn");
	}
	void StartTargetSelection();
	void MoveFingerToBattlerLocation(Battler* battler);
	void CloseTargetSelection();
	void UpdateTargetInfo(const std::string& displayName);
	void MoveCursorInMenu(unsigned int newLocation);
	void OpenMagicMenu();
	void CloseMagicMenu();
	void MoveCursorInMagicMenu(unsigned int col, unsigned int row);
	void UpdateAnimations();
	void StartPlayerTurn(Battler* battler);
	void EndPlayerTurn(Battler* battler);
	static void ClosePlayerInfoBox();

   private:
	static UIAnimation* _turnMarkerAnim;
	UIObject* _commandMenu;
	UIObject* _menuItems[4];
	UIObject* _magicMenu = nullptr;
	UIObject* _magicMenuItems[8] = {};
	int _magicMenuRows = 4;
	int _magicMenuCols = 2;
	UIImage* _magicFinger = nullptr;
	UIText* _hpObject;
	UIImage* _finger;
	UIImage* _targetSelectionFinger;
	UINineSlice* _targetInfoBox;
	UIText* _targetInfoText;
	UIAnimation* _progressBarAnim;
	UIProgressBar* _progressBar;
	bool _player;
	unsigned int _battlerNum;
	float _currentAnimationTime = 0;
	float _magicAnimationTime = 0;
	float _menuBoxStartX, _menuBoxStartY;
	float _magicMenuStartX = 0;
	unsigned int _pendingMagicCol = 0;
	unsigned int _pendingMagicRow = 0;
	PlayerUIAnimationStates _currentState = PlayerUIAnimationStates::Closed;
	PlayerUIAnimationStates _magicMenuState = PlayerUIAnimationStates::Closed;
};

}  // namespace Etf
