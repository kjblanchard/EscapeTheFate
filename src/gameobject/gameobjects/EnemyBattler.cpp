#include <Supergoon/Graphics/shader.h>
#include <cstdlib>
#include <engine.hpp>
#include <gameState.hpp>
#include <sgtools/log.h>
#include <gameobject/gameobjects/EnemyBattler.hpp>
#include <systems/battleSystem.hpp>

using namespace Etf;
using namespace std;
using enum EnemyBattlerStates;

EnemyBattler::EnemyBattler(const BattlerArgs& args) : Battler(args) {
	_enemyUI = make_unique<EnemyBattlerUI>(args.BattlerNum, this);
	_deathShader = ShaderCreate();
	ShaderCompile(_deathShader, "2dSpriteVertex", "deathDissolveFragment");
}

EnemyBattler::~EnemyBattler() {
	sgLogDebug("Destroying enemy battler");
	if (_deathShader) {
		if (_sprite && _sprite->Shader == _deathShader) {
			_sprite->Shader = GetDefaultShader();
		}
		ShaderDestroy(_deathShader);
		_deathShader = nullptr;
	}
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
			_enemyUI->SetVisible(false);
		}
		return;
	}

	if (_currentHP < 1) return;

	switch (_currentState) {
		case Default:
			_currentState = ATBCharging;
			_enemyUI->StartATBIdleAnim();
			break;

		case ATBCharging: {
			float progress = _currentATBCharge / _maxATBCharge * 100.0f;
			_enemyUI->UpdateATBBar(progress);
			if (_currentATBCharge >= _maxATBCharge) {
				_currentState = ATBFullyCharged;
			}
			break;
		}

		case ATBFullyCharged:
			_enemyUI->StartATBTurnAnim();
			_currentState = Attacking;
			break;

		case Attacking: {
			auto& battlers = BattleSystem::GetEnemyBattlers();
			Battler* target = nullptr;
			for (auto b : battlers) {
				if (b && b->IsPlayer() && b->CurrentHP() > 0) {
					target = b;
					break;
				}
			}
			if (target) {
				target->TakeDamage(_battlerData->Str);
			}
			_currentATBCharge = 0;
			_currentState = ATBCharging;
			_enemyUI->StartATBIdleAnim();
			break;
		}
	}
}

void EnemyBattler::takeDamageImpl(int damage) {
	_enemyUI->UpdateHP((float)_currentHP / _battlerData->HP * 100.0f);
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
