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
}  // namespace

EnemyBattler::EnemyBattler(const BattlerArgs& args) : Battler(args) {
	_ai.reset(CreateEnemyAI(args.BattleData->AIStrategy));
	_deathShader = ShaderCreate();
	ShaderCompile(_deathShader, "2dSpriteVertex", "deathDissolveFragment");

	constexpr float kBarWidth = 18;
	constexpr float kBarHeight = 3;
	constexpr float kPadding = 1;
	constexpr float kPanelW = kBarWidth + kPadding;
	constexpr float kPanelH = kBarHeight * 2 + kPadding;

	float panelX = (int)(X() + (SpriteWidth() / 2) - (kPanelW / 2) + battlerData->HpBarOffsetX);
	float panelY = (int)(Y() + SpriteHeight() + battlerData->HpBarOffsetY);

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

	// auto p = StatusEffectInstance();
	// p.StatusType = StatusEffects::Poison;
	// p.Duration = 10;
	// statusEffects.push_back(p);
}

EnemyBattler::~EnemyBattler() {
	sgLogDebug("Destroying enemy battler");
	if (_barPanel) {
		_barPanel->SetVisible(false);
	}
	if (_deathShader) {
		if (sprite && sprite->Shader == _deathShader) {
			sprite->Shader = GetDefaultShader();
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

void EnemyBattler::getEnemyAllies(std::vector<Battler*>& out) {
	auto& all = BattleSystem::GetEnemyBattlers();
	copy_if(all.begin(), all.end(), back_inserter(out), [](Battler* b) {
		return b && !b->IsPlayer() && b->CurrentHP() > 0;
	});
}

void EnemyBattler::onAPGained() {
	if (_enemyState != ATBCharging) return;

	vector<Battler*> players;
	vector<Battler*> allies;
	getPlayerBattlers(players);
	getEnemyAllies(allies);

	AIAction action = _ai->SelectAction(this, allies, players);
	if (action.AbilityID < 0 || !action.Target) return;

	_pendingAction = action;
	_attackDelay = 0.8f + (rand() % 700) / 1000.0f;
	_attackDelayTimer = 0.0f;
	_enemyState = DelayBeforeAttack;
}

void EnemyBattler::updateImpl() {
	if (_deathEffectPlaying) {
		_deathEffectTime += GameState::DeltaTimeSeconds;
		float t = _deathEffectTime / kDeathEffectDuration;
		if (t > 1.0f) t = 1.0f;
		ShaderSetUniformFloat(_deathShader, "time", t, 1);
		if (_deathEffectTime >= kDeathEffectDuration) {
			Engine::Sprites::SetSpriteVisible(sprite, false);
			sprite->Shader = GetDefaultShader();
			ShaderDestroy(_deathShader);
			_deathShader = nullptr;
			_deathEffectPlaying = false;
			if (_barPanel) _barPanel->SetVisible(false);
		}
		return;
	}
	if (currentHP < 1) return;

	switch (_enemyState) {
		case ATBCharging: {
			auto progress = currentATBCharge / maxATBCharge * 100.0f;
			if (_atbProgressBar) _atbProgressBar->SetBarPercent(progress);
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
				sprite->DrawColor = _blinkDark ? kBlack : kWhite;
				if (!_blinkDark) {
					++_blinkCount;
				}
			}
			if (_blinkCount >= kBlinkTotal) {
				sprite->DrawColor = kWhite;
				_enemyState = Attacking;
			}
			break;
		}
		case Attacking: {
			auto target = _pendingAction.Target;
			if (target && target->CurrentHP() > 0) {
				const auto& ability = BattleSystem::GetAbilityByID(_pendingAction.AbilityID);
				if (!ability.PlayerAnim.empty() && animator->GetAnimationDuration(ability.PlayerAnim) > 0) {
					animator->PlayAnimationThenLoopSecond(ability.PlayerAnim, battlerData->IdleAnimation);
				}
				if (ability.BaseDamage < 0) {
					target->Heal(-ability.BaseDamage);
				} else {
					target->TakeDamage(ability.BaseDamage);
				}
				target->PlayHitAnimation(ability);
				spendAP(ability.APCost);
			}
			_pendingAction = {};
            handleTurnEndStatus();
			currentATBCharge = 0;
			if (_atbProgressBar) _atbProgressBar->SetBarPercent(0);
			_enemyState = ATBCharging;
			break;
		}
	}
}

void EnemyBattler::healImpl(int amount) {
	float hpPercent = (float)currentHP / (float)battlerData->HP * 100.0f;
	if (hpPercent > 100) hpPercent = 100;
	if (_hpProgressBar) _hpProgressBar->SetBarPercent(hpPercent);
}

void EnemyBattler::takeDamageImpl(int damage) {
	float hpPercent = (float)currentHP / (float)battlerData->HP * 100.0f;
	if (hpPercent < 0) hpPercent = 0;
	if (_hpProgressBar) _hpProgressBar->SetBarPercent(hpPercent);
	if (currentHP < 1 && !_deathEffectPlaying) {
		if (_barPanel) _barPanel->SetVisible(false);
		Engine::Audio::PlaySFXBuffer("enemyDead", 1.0);
		_deathEffectPlaying = true;
		_deathEffectTime = 0.0f;
		sprite->Shader = _deathShader;
		float randomSeed = (float)(rand() % 1000) / 1000.0f;
		ShaderSetUniformFloat(_deathShader, "seed", randomSeed, 1);
		ShaderSetUniformFloat(_deathShader, "time", 0.0f, 1);
	}
}
