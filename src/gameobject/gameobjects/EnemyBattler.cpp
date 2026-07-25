#include <Supergoon/Graphics/shader.h>
#include <cstdlib>
#include <engine.hpp>
#include <gameState.hpp>
#include <sgtools/log.h>
#include <gameobject/gameobjects/EnemyBattler.hpp>
#include <ui/ui.hpp>

using namespace Etf;
using namespace std;

EnemyBattler::EnemyBattler(const BattlerArgs& args) : Battler(args) {
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

void EnemyBattler::updateImpl() {
	if (!_deathEffectPlaying) return;
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
