#include <battle/enemyBattlerUI.hpp>
#include <format>
#include <gameobject/gameobjects/Battler.hpp>
#include <ui/ui.hpp>

using namespace Etf;
using namespace std;

EnemyBattlerUI::EnemyBattlerUI(unsigned int enemyIndex, Battler* battler) {
	_hpBarName = format("Enemy{}HP", enemyIndex);
	_atbBarName = format("Enemy{}ATB", enemyIndex);
	auto atbProgressName = format("Enemy{}ATBProgress", enemyIndex);

	float centerX = battler->X() + battler->SpriteWidth() / 2.0f;
	float belowY = battler->Y() + battler->SpriteHeight();

	UIProgressBarArgs hpArgs;
	hpArgs.Name = _hpBarName;
	hpArgs.Rect = {centerX - 16, belowY + 2, 32, 5};
	hpArgs.BarRect = {1, 1, 30, 3};
	hpArgs.BarColor = {77, 255, 77, 255};
	hpArgs.Priority = 3;
	hpArgs.Visible = true;
	_hpBar = new UIProgressBar(hpArgs);
	_hpBar->SetBarPercent(100.0f);
	UI::GetRootUIObject()->AddChild(_hpBar);

	UIAnimationArgs atbAnimArgs;
	atbAnimArgs.Name = _atbBarName;
	atbAnimArgs.Filename = "atbBar";
	atbAnimArgs.Rect = {centerX - 16, belowY + 8, 32, 16};
	atbAnimArgs.SourceRect = {0, 0, 16, 16};
	atbAnimArgs.Scale = 1.0f;
	atbAnimArgs.DrawColor = {255, 255, 255, 255};
	atbAnimArgs.Priority = 3;
	atbAnimArgs.Visible = true;
	_atbBarAnim = new UIAnimation(atbAnimArgs);

	UIProgressBarArgs atbBarArgs;
	atbBarArgs.Name = atbProgressName;
	atbBarArgs.Rect = {0, 0, 32, 16};
	atbBarArgs.BarRect = {4, 6, 24, 3};
	atbBarArgs.BarColor = {77, 77, 255, 255};
	atbBarArgs.Priority = 0;
	atbBarArgs.Visible = true;
	_atbProgressBar = new UIProgressBar(atbBarArgs);
	_atbBarAnim->AddChild(_atbProgressBar);

	UI::GetRootUIObject()->AddChild(_atbBarAnim);
	StartATBIdleAnim();
}

EnemyBattlerUI::~EnemyBattlerUI() {
	UI::GetRootUIObject()->DestroyChildByName(_hpBarName, true);
	UI::GetRootUIObject()->DestroyChildByName(_atbBarName, true);
}

void EnemyBattlerUI::UpdateHP(float percent) {
	if (_hpBar) _hpBar->SetBarPercent(percent);
}

void EnemyBattlerUI::UpdateATBBar(float percent) {
	if (_atbProgressBar) _atbProgressBar->SetBarPercent(percent);
}

void EnemyBattlerUI::StartATBIdleAnim() {
	if (_atbBarAnim) _atbBarAnim->GetAnimator().StartAnimation("idle");
}

void EnemyBattlerUI::StartATBTurnAnim() {
	if (_atbBarAnim) _atbBarAnim->GetAnimator().StartAnimation("turn");
}

void EnemyBattlerUI::SetVisible(bool visible) {
	if (_hpBar) _hpBar->SetVisible(visible);
	if (_atbBarAnim) _atbBarAnim->SetVisible(visible);
}
