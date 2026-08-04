#include <Supergoon/Graphics/shader.h>
#include <Supergoon/sprite.h>
#include <sgtools/log.h>

#include <algorithm>
#include <cstdlib>
#include <engine.hpp>
#include <gameState.hpp>
#include <gameobject/gameobjects/EnemyBattler.hpp>
#include <iterator>
#include <systems/battleSystem.hpp>
#include <ui/ui.hpp>
#include <ui/uiLine.hpp>
#include <ui/uiPanel.hpp>
#include <ui/uiProgressBar.hpp>

using namespace Etf;
using namespace std;
using enum EnemyBattlerStates;

namespace {
constexpr Color kWhite = {255, 255, 255, 255};
constexpr Color kBlack = {0, 0, 0, 255};
constexpr Color kEnemyPanelBorderColor = {255, 235, 235, 255};
}


EnemyBattler::EnemyBattler(const BattlerArgs& args) : Battler(args) {
	_deathShader = ShaderCreate();
	ShaderCompile(_deathShader, "2dSpriteVertex", "deathDissolveFragment");

	constexpr float kBarWidth = 18;
	constexpr float kBarHeight = 3;
	constexpr float kPadding = 1;
	constexpr float kPanelW = kBarWidth + kPadding;
	constexpr float kPanelH = kBarHeight * 2 + kPadding;

	float panelX = X() + (SpriteWidth() / 2) - (kPanelW / 2) + _battlerData->HpBarOffsetX;
	float panelY = Y() + SpriteHeight() + _battlerData->HpBarOffsetY;

	UIPanelArgs panelArgs;
	panelArgs.Name = "EnemyBarPanel";
	panelArgs.Rect = {panelX, panelY, kPanelW, kPanelH};
	panelArgs.FillColor = {0, 0, 0, 0};
	panelArgs.BorderColor = kEnemyPanelBorderColor;
	panelArgs.Priority = 0;
	panelArgs.Visible = true;
	_barPanel = new UIPanel(panelArgs);
	UI::GetRootUIObject()->GetChildByName("BattlePanel")->AddChild(_barPanel);

	UIProgressBarArgs hpBarArgs;
	hpBarArgs.Name = "EnemyHPBar";
	hpBarArgs.Rect = {0, 0, kBarWidth, kBarHeight};
	hpBarArgs.BarRect = {0, 0, kBarWidth, kBarHeight};
	hpBarArgs.BarColor = {50, 200, 50, 255};
	hpBarArgs.BackgroundColor = {30, 30, 30, 255};
	hpBarArgs.Priority = 1;
	hpBarArgs.Visible = true;
	_hpProgressBar = new UIProgressBar(hpBarArgs);
	_hpProgressBar->SetBarPercent(100.0f);
	_barPanel->AddChild(_hpProgressBar);

	UIProgressBarArgs pbArgs;
	pbArgs.Name = "EnemyATBProgress";
	pbArgs.Rect = {0, kBarHeight, kBarWidth, kBarHeight};
	pbArgs.BarRect = {0, 0, kBarWidth, kBarHeight};
	pbArgs.BarColor = {255, 140, 0, 255};
	pbArgs.BackgroundColor = {30, 30, 30, 255};
	pbArgs.Priority = 1;
	pbArgs.Visible = true;
	_atbProgressBar = new UIProgressBar(pbArgs);
	_barPanel->AddChild(_atbProgressBar);
}

EnemyBattler::~EnemyBattler() {
	sgLogDebug("Destroying enemy battler");
	if (_barPanel) {
		_barPanel->SetVisible(false);
	}
	if (_deathShader) {
		if (_sprite && _sprite->Shader == _deathShader) {
			_sprite->Shader = GetDefaultShader();
		}
		ShaderDestroy(_deathShader);
		_deathShader = nullptr;
	}
}

void EnemyBattler::getPlayerBattlers(std::vector<Battler*>& out) {
	auto& all = BattleSystem::GetEnemyBattlers();
	copy_if(all.begin(), all.end(), back_inserter(out), [](Battler* b) {
		return b && b->IsPlayer() && b->CurrentHP() > 0;
	});
}

void EnemyBattler::updateImpl() {
	if (_deathEffectPlaying) {
		_deathEffectTime += GameState::DeltaTimeSeconds;
		float t = _deathEffectTime / kDeathEffectDuration;
		if (t > 1.0f) t = 1.0f;
		ShaderSetUniformFloat(_deathShader, "time", t, 1);
		if (_deathEffectTime >= kDeathEffectDuration) {
			Engine::Sprites::SetSpriteVisible(_sprite, false);
			_sprite->Shader = GetDefaultShader();
			ShaderDestroy(_deathShader);
			_deathShader = nullptr;
			_deathEffectPlaying = false;
			if (_barPanel) _barPanel->SetVisible(false);
		}
		return;
	}
	if (_currentHP < 1) return;

	switch (_enemyState) {
		case ATBCharging: {
			auto progress = _currentATBCharge / _maxATBCharge * 100.0f;
			if (_atbProgressBar) _atbProgressBar->SetBarPercent(progress);
			if (_currentATBCharge >= _maxATBCharge) {
				_attackDelay = 0.8f + (rand() % 700) / 1000.0f;
				_attackDelayTimer = 0.0f;
				_enemyState = DelayBeforeAttack;
			}
			break;
		}
		case DelayBeforeAttack: {
			_attackDelayTimer += GameState::DeltaTimeSeconds;
			if (_attackDelayTimer >= _attackDelay) {
				_blinkToggleTimer = 0.0f;
				_blinkDark = false;
				_blinkCount = 0;
				_enemyState = Blinking;
			}
			break;
		}
		case Blinking: {
			_blinkToggleTimer += GameState::DeltaTimeSeconds;
			float threshold = _blinkDark ? kBlinkDarkTime : kBlinkLightTime;
			if (_blinkToggleTimer >= threshold) {
				_blinkToggleTimer = 0.0f;
				_blinkDark = !_blinkDark;
				_sprite->DrawColor = _blinkDark ? kBlack : kWhite;
				if (!_blinkDark) {
					++_blinkCount;
				}
			}
			if (_blinkCount >= kBlinkTotal) {
				_sprite->DrawColor = kWhite;
				_enemyState = Attacking;
			}
			break;
		}
		case Attacking: {
			vector<Battler*> players;
			getPlayerBattlers(players);
			if (!players.empty()) {
				auto target = players[0];
				const auto& ability = BattleSystem::GetAbilityByID(2);
				if (!ability.PlayerAnim.empty() && _animator->GetAnimationDuration(ability.PlayerAnim) > 0) {
					_animator->PlayAnimationThenLoopSecond(ability.PlayerAnim, _battlerData->IdleAnimation);
				}
				target->TakeDamage(ability.BaseDamage);
				target->PlayHitAnimation(ability);
			}
			_currentATBCharge = 0;
			if (_atbProgressBar) _atbProgressBar->SetBarPercent(0);
			_enemyState = ATBCharging;
			break;
		}
	}
}

void EnemyBattler::healImpl(int amount) {
	float hpPercent = (float)_currentHP / (float)_battlerData->HP * 100.0f;
	if (hpPercent > 100) hpPercent = 100;
	if (_hpProgressBar) _hpProgressBar->SetBarPercent(hpPercent);
}

void EnemyBattler::takeDamageImpl(int damage) {
	float hpPercent = (float)_currentHP / (float)_battlerData->HP * 100.0f;
	if (hpPercent < 0) hpPercent = 0;
	if (_hpProgressBar) _hpProgressBar->SetBarPercent(hpPercent);
	if (_currentHP < 1 && !_deathEffectPlaying) {
		if (_barPanel) _barPanel->SetVisible(false);
		Engine::Audio::PlaySFXBuffer("enemyDead", 1.0);
		_deathEffectPlaying = true;
		_deathEffectTime = 0.0f;
		_sprite->Shader = _deathShader;
		float randomSeed = (float)(rand() % 1000) / 1000.0f;
		ShaderSetUniformFloat(_deathShader, "seed", randomSeed, 1);
		ShaderSetUniformFloat(_deathShader, "time", 0.0f, 1);
	}
}
