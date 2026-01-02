#include <Supergoon/Animation/animator.h>
#include <Supergoon/Audio/Audio.h>
#include <Supergoon/Graphics/shader.h>
#include <Supergoon/Graphics/texture.h>
#include <Supergoon/log.h>
#include <Supergoon/map.h>
#include <Supergoon/sprite.h>

#include <algorithm>
#include <bindings/engine.hpp>
#include <gameConfig.hpp>
#include <gameobject/GameObject.hpp>

using namespace Etf;
using namespace std;

void Engine::LoadAndPlayBGM(const std::string& name, float volume) {
	SetBgmTrack(0);
	LoadBgm(name.c_str(), volume, -1);
	PlayBgm();
}
void Engine::LoadScene(const string& name) {
	auto newName = name;
	auto& gameSceneConfig = GameConfig::GetGameConfig().scene;
	if (newName == "") {
		newName = gameSceneConfig.defaultScene;
	}
	const auto it = std::find_if(gameSceneConfig.scenes.begin(), gameSceneConfig.scenes.end(), [&newName](Scene& scene) {
		return scene.MapName == newName;
	});
	if (it == gameSceneConfig.scenes.end()) {
		sgLogWarn("Could not find scene with name %s, not loading", name.c_str());
		return;
	}
	auto& sceneToLoad = *it;
	LoadMap(newName.c_str());
	GameObject::LoadAllGameObjects();
	LoadAndPlayBGM(sceneToLoad.BGMName, sceneToLoad.BGMVolume);
}

Sprite* Engine::CreateSpriteFull(const std::string& name, sgGameObject* parent, RectangleF sourceRect, RectangleF offsetSizeRect) {
	auto sprite = NewSprite();
	sprite->Parent = parent;
	sprite->Flags |= SpriteFlagVisible;
	sprite->Texture = TextureCreate(name.c_str());
	TextureLoadFromBmp(sprite->Texture, name.c_str());
	sprite->Shader = GetDefaultShader();
	sprite->TextureSourceRect = sourceRect;
	sprite->OffsetAndSizeRectF = offsetSizeRect;
	return sprite;
}

unsigned int Engine::CreateAnimatorFull(const std::string& name, Sprite* sprite) {
	auto animator = CreateAnimator(name.c_str());
	_animators.Animators[animator].Sprite = sprite;
	return animator;
}

void Engine::StartAnimatorAnimation(unsigned int animator, const char* animName, float animSpeed) {
	SetAnimatorAnimationSpeed(animator, animSpeed);
	PlayAnimation(animator, animName, -1);
}

void Engine::UpdateAnimatorAnimationSpeed(unsigned int animator, float animSpeed) {
	SetAnimatorAnimationSpeed(animator, animSpeed);
}
