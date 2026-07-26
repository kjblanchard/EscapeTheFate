#include <Supergoon/Graphics/shader.h>
#include <Supergoon/sprite.h>
#include <cstdlib>
#include <engine.hpp>
#include <gameState.hpp>
#include <sgtools/log.h>
#include <gameobject/gameobjects/EnemyBattler.hpp>
#include <systems/battleSystem.hpp>
#include <ui/ui.hpp>
#include <ui/uiProgressBar.hpp>
#include <algorithm>
#include <iterator>

using namespace Etf;
using namespace std;
using enum EnemyBattlerStates;

static const Color kWhite = {255, 255, 255, 255};
static const Color kBlack = {0, 0, 0, 255};

EnemyBattler::EnemyBattler(const BattlerArgs& args) : Battler(args) {
	_deathShader = ShaderCreate();
	ShaderCompile(_deathShader, "2dSpriteVertex", "deathDissolveFragment");

	float barX = X() + (SpriteWidth() / 2) - 12;
	float barY = Y() + SpriteHeight() + 2;

	UIProgressBarArgs hpBarArgs;
	hpBarArgs.Name = "EnemyHPBar";
	hpBarArgs.Rect = {barX, barY, 24, 4};
	hpBarArgs.BarRect = {1, 1, 22, 2};
	hpBarArgs.BarColor = {50, 200, 50, 255};
	hpBarArgs.BackgroundColor = {20, 20, 20, 255};
	hpBarArgs.Priority = 1;
	hpBarArgs.Visible = true;
	_hpProgressBar = new UIProgressBar(hpBarArgs);
	_hpProgressBar->SetBarPercent(100.0f);
	UI::GetRootUIObject()->GetChildByName("BattlePanel")->AddChild(_hpProgressBar);

	UIProgressBarArgs pbArgs;
	pbArgs.Name = "EnemyATBProgress";
	pbArgs.Rect = {barX, barY + 5, 24, 4};
	pbArgs.BarRect = {1, 1, 22, 2};
	pbArgs.BarColor = {255, 140, 0, 255};
	pbArgs.BackgroundColor = {20, 20, 20, 255};
	pbArgs.Priority = 1;
	pbArgs.Visible = true;
	_atbProgressBar = new UIProgressBar(pbArgs);
	UI::GetRootUIObject()->GetChildByName("BattlePanel")->AddChild(_atbProgressBar);
}

EnemyBattler::~EnemyBattler() {
	sgLogDebug("Destroying enemy battler");
	if (_hpProgressBar) {
		_hpProgressBar->SetVisible(false);
	}
	if (_atbProgressBar) {
		_atbProgressBar->SetVisible(false);
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
			if (_hpProgressBar) _hpProgressBar->SetVisible(false);
			if (_atbProgressBar) _atbProgressBar->SetVisible(false);
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
				target->TakeDamage(1);
			}
			_currentATBCharge = 0;
			if (_atbProgressBar) _atbProgressBar->SetBarPercent(0);
			_enemyState = ATBCharging;
			break;
		}
	}
}

void EnemyBattler::takeDamageImpl(int damage) {
	float hpPercent = (float)_currentHP / (float)_battlerData->HP * 100.0f;
	if (hpPercent < 0) hpPercent = 0;
	if (_hpProgressBar) _hpProgressBar->SetBarPercent(hpPercent);
	if (_currentHP < 1 && !_deathEffectPlaying) {
		Engine::Audio::PlaySFXBuffer("enemyDead", 1.0);
		_deathEffectPlaying = true;
		_deathEffectTime = 0.0f;
		_sprite->Shader = _deathShader;
		float randomSeed = (float)(rand() % 1000) / 1000.0f;
		ShaderSetUniformFloat(_deathShader, "seed", randomSeed, 1);
		ShaderSetUniformFloat(_deathShader, "time", 0.0f, 1);
	}
}
