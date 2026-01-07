#include <Supergoon/Animation/animator.h>
#include <Supergoon/Audio/Audio.h>
#include <Supergoon/Graphics/graphics.h>
#include <Supergoon/Graphics/shader.h>
#include <Supergoon/Graphics/texture.h>
#include <Supergoon/log.h>
#include <Supergoon/map.h>
#include <Supergoon/sprite.h>
#include <Supergoon/text.h>

#include <ui/ui.hpp>
#include <algorithm>
#include <bindings/engine.hpp>
#include <gameConfig.hpp>
#include <gameobject/GameObject.hpp>

using namespace Etf;
using namespace std;

std::string Engine::_nextScene = "";
void Engine::LoadAndPlayBGM(const std::string& name, float volume) {
	SetBgmTrack(0);
	LoadBgm(name.c_str(), volume, -1);
	PlayBgm();
}

void Engine::loadSceneInternal() {
	auto& gameSceneConfig = GameConfig::GetGameConfig().scene;
	const auto it = std::find_if(gameSceneConfig.scenes.begin(), gameSceneConfig.scenes.end(), [](Scene& scene) {
		return scene.MapName == _nextScene;
	});
	if (it == gameSceneConfig.scenes.end()) {
		sgLogWarn("Could not find scene with name %s, not loading", _nextScene.c_str());
		return;
	}
	auto& sceneToLoad = *it;
	// We should destroy all of the old gameobjects, and also load the ui if needed.
	LoadMap(_nextScene.c_str());
	GameObject::LoadAllGameObjects();
	LoadAndPlayBGM(sceneToLoad.BGMName, sceneToLoad.BGMVolume);
	UI::LoadUIFromFile("assets/ui/debugTown.jsonc");
	_nextScene = "";
}

void Engine::LoadScene(const string& name) {
	auto newName = name;
	auto& gameSceneConfig = GameConfig::GetGameConfig().scene;
	if (newName == "") {
		newName = gameSceneConfig.defaultScene;
	}
	_nextScene = newName;
}

void Engine::HandleMapLoad() {
	if (_nextScene == "") return;
	loadSceneInternal();
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

unsigned int Engine::Animation::CreateAnimatorFull(const std::string& name, Sprite* sprite) {
	auto animator = CreateAnimator(name.c_str());
	_animators.Animators[animator].Sprite = sprite;
	return animator;
}

void Engine::Animation::StartAnimatorAnimation(unsigned int animator, const char* animName, float animSpeed) {
	SetAnimatorAnimationSpeed(animator, animSpeed);
	PlayAnimation(animator, animName, -1);
}

void Engine::Animation::UpdateAnimatorAnimationSpeed(unsigned int animator, float animSpeed) {
	SetAnimatorAnimationSpeed(animator, animSpeed);
}

void Engine::Animation::DestroyAnimatorFull(unsigned int animator) {
	DestroyAnimator(animator);
}

void Engine::DrawRectPrimitive(RectangleF& rect, Color color, bool filled, bool cameraOffset) {
	DrawRect(&rect, &color, filled, cameraOffset);
}

void Engine::SetSpriteVisible(Sprite* sprite, bool visible) {
	if (visible)
		sprite->Flags |= SpriteFlagVisible;	 // set bit
	else
		sprite->Flags &= ~SpriteFlagVisible;  // clear bit
}

Text* Engine::TextBoi::CreateText(const std::string& fontName, unsigned int fontSize, RectangleF location, const std::string& text, unsigned int numChars, bool centeredX, bool centeredY) {
	TextSetFont(fontName.c_str(), fontSize);
	auto textPtr = TextCreate(&location, text.c_str());
	textPtr->NumLettersToDraw = numChars;
	textPtr->CenteredX = centeredX;
	textPtr->CenteredY = centeredY;
	textPtr->WordWrap = true;
	TextLoad(textPtr);
	TextOnDirty(textPtr);
	return textPtr;
}

void Engine::TextBoi::DrawText(Text* text, float xOffset, float yOffset) {
	TextDraw(text, xOffset, yOffset);
}
