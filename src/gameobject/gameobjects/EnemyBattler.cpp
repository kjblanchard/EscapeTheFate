#include <Supergoon/Graphics/shader.h>
#include <cstdlib>
#include <engine.hpp>
#include <gameState.hpp>
#include <sgtools/log.h>
#include <gameobject/gameobjects/EnemyBattler.hpp>
#include <systems/battleSystem.hpp>
#include <ui/ui.hpp>
#include <algorithm>
#include <iterator>

using namespace Etf;
using namespace std;
using enum EnemyBattlerStates;

EnemyBattler::EnemyBattler(const BattlerArgs& args) : Battler(args), _battlerUI(make_unique<BattlerUI>(3)) {
	auto hpObject = UI::GetRootUIObject()->GetChildByName("EnemyHP");
	_hpObject = static_cast<UIText*>(hpObject);
	_hpObject->UpdateText(to_string(_currentHP));
	_deathShader = ShaderCreate();
	ShaderCompile(_deathShader, "2dSpriteVertex", "deathDissolveFragment");
}

EnemyBattler::~EnemyBattler() {
	sgLogDebug("Destroying enemy battler");
	if (_hpObject) {
		_hpObject->SetVisible(false);
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
		}
		return;
	}
	if (_currentHP < 1) return;

	switch (_enemyState) {
		case ATBCharging: {
			auto progress = _currentATBCharge / _maxATBCharge * 100.0f;
			_battlerUI->UpdateProgressBar(progress);
			if (_currentATBCharge >= _maxATBCharge) {
				_battlerUI->StartATBTurnAnim();
				_attackDelay = 0.3f + (rand() % 500) / 1000.0f;
				_attackDelayTimer = 0.0f;
				_enemyState = DelayBeforeAttack;
			}
			break;
		}
		case DelayBeforeAttack: {
			_attackDelayTimer += GameState::DeltaTimeSeconds;
			if (_attackDelayTimer >= _attackDelay) {
				_blinkTimer = 0.0f;
				_blinkToggleTimer = 0.0f;
				_blinkVisible = true;
				_enemyState = Blinking;
			}
			break;
		}
		case Blinking: {
			_blinkTimer += GameState::DeltaTimeSeconds;
			_blinkToggleTimer += GameState::DeltaTimeSeconds;
			if (_blinkToggleTimer >= kBlinkToggleInterval) {
				_blinkToggleTimer = 0.0f;
				_blinkVisible = !_blinkVisible;
				Engine::Sprites::SetSpriteVisible(_sprite, _blinkVisible);
			}
			if (_blinkTimer >= kBlinkDuration) {
				Engine::Sprites::SetSpriteVisible(_sprite, true);
				_enemyState = Attacking;
			}
			break;
		}
		case Attacking: {
			vector<Battler*> players;
			getPlayerBattlers(players);
			if (!players.empty()) {
				auto target = players[0];
				int damage = max(1, _battlerData->Str - target->Def() / 2);
				target->TakeDamage(damage);
			}
			_currentATBCharge = 0;
			_battlerUI->StartATBIdleAnim();
			_battlerUI->UpdateProgressBar(0);
			_enemyState = ATBCharging;
			break;
		}
	}
}

void EnemyBattler::takeDamageImpl(int damage) {
	_hpObject->UpdateText(to_string(_currentHP));
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
