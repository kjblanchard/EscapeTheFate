#include <Supergoon/Animation/animator.h>
#include <Supergoon/Audio/Audio.h>
#include <Supergoon/Graphics/graphics.h>
#include <Supergoon/Graphics/shader.h>
#include <Supergoon/Graphics/texture.h>
#include <Supergoon/Tweening/easing.h>
#include <Supergoon/camera.h>
#include <Supergoon/filesystem.h>
#include <Supergoon/json.h>
#include <Supergoon/log.h>
#include <Supergoon/map.h>
#include <Supergoon/sprite.h>
#include <Supergoon/text.h>

#include <algorithm>
#include <bindings/engine.hpp>
#include <format>
#include <gameConfig.hpp>
#include <gameState.hpp>
#include <gameobject/GameObject.hpp>
#include <systems/dialogSystem.hpp>
#include <ui/ui.hpp>

using namespace Etf;
using namespace std;

string Engine::_currentScene = "";
string Engine::_nextScene = "";
static string _currentBGM = "";

void Engine::PlaySFX(const std::string& name, float volume) {
	PlaySfxOneShot(name.c_str(), volume);
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
	ResetCameraFollow();
	LoadMap(_nextScene.c_str());
	GameObject::LoadAllGameObjects();
	if (_currentBGM != sceneToLoad.BGMName) {
		Engine::Audio::PlayBGM(sceneToLoad.BGMName, sceneToLoad.BGMVolume);
		_currentBGM = sceneToLoad.BGMName;
	}
	if (!sceneToLoad.UIName.empty()) {
		UI::LoadUIFromFile(format("{}assets/ui/{}.json", GetBasePath(), sceneToLoad.UIName));
	} else {
		UI::RootUIObject->DestroyChildIfNotName("");
	}
	DialogSystem::LoadDialogFromJsonFile(sceneToLoad.MapName);
	_currentScene = _nextScene;
	_nextScene = "";
	GameState::NextLoadMapName = "";
	GameState::Battle::ExitingFromBattle = false;
}

void Engine::LoadScene(const string& name) {
	auto newName = name;
	auto& gameSceneConfig = GameConfig::GetGameConfig().scene;
	if (newName.empty()) {
		newName = gameSceneConfig.defaultScene;
	}
	_nextScene = newName;
	// Reset the battle, this is used when loading player battlers
}

void Engine::HandleMapLoad() {
	if (_nextScene.empty()) return;
	loadSceneInternal();
}


Sprite* Engine::CreateSpriteFull(const std::string& name, float* followX, float* followY, RectangleF sourceRect, RectangleF offsetSizeRect) {
	auto sprite = NewSprite();
	sprite->parentX = followX;
	sprite->parentY = followY;
	sprite->Flags |= SpriteFlagVisible;
	sprite->Texture = TextureCreate(name.c_str());
	TextureLoadFromPng(sprite->Texture, name.c_str());
	sprite->Shader = GetDefaultShader();
	sprite->TextureSourceRect = sourceRect;
	sprite->OffsetAndSizeRectF = offsetSizeRect;
	return sprite;
}

//TODO this should be refactored instead of copy/paste from createspritefull
Sprite* Engine::CreateManualSpriteFull(const std::string& name, float* followX, float* followY, RectangleF sourceRect, RectangleF offsetSizeRect) {
	auto sprite = NewSpriteManual();
	sprite->parentX = followX;
	sprite->parentY = followY;
	sprite->Flags |= SpriteFlagVisible;
	sprite->Texture = TextureCreate(name.c_str());
	TextureLoadFromPng(sprite->Texture, name.c_str());
	sprite->Shader = GetDefaultShader();
	sprite->TextureSourceRect = sourceRect;
	sprite->OffsetAndSizeRectF = offsetSizeRect;
	return sprite;
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

void Engine::TextBoi::DrawText(Text* text, float xOffset, float yOffset, Color& color) {
	TextDraw(text, xOffset, yOffset, &color);
}

float Engine::Tweening::GetTweenedValue(float start, float end, float timeSeconds, float totalSeconds, TweenEaseTypes ease) {
	auto func = geLinearInterpolation;
	switch (ease) {
		case TweenEaseTypes::QuintOut:
			func = geQuinticEaseOut;
		default:
			break;
	}
	float progressPercent = func(timeSeconds / totalSeconds);
	float progressValue = start + ((end - start) * progressPercent);
	// clamp to edges
	if (start < end)
		progressValue = std::min(progressValue, end);
	else
		progressValue = std::max(progressValue, end);
	return progressValue;
}

void Engine::Audio::PlayBGM(const std::string& name, float volume) {
	SetBgmTrack(0);
	LoadBgm(name.c_str(), volume, -1);
	PlayBgm();
}

void Engine::Audio::PlayBGMBackground(const std::string& name, float volume) {
	SetBgmTrack(1);
	LoadBgm(name.c_str(), volume, -1);
	PlayBgm();
}

void Engine::Audio::StopBGMBackground() {
	SetBgmTrack(1);
	StopBgm();
}

RectangleF Engine::Json::GetRectFromObject(void* object, const std::string& key) {
	auto obj = static_cast<json_object*>(object);
	auto rectJson = jobj(obj, key.c_str());
	if (!rectJson) return {0, 0, 0, 0};
	return {
		jfloat(rectJson, "x"),
		jfloat(rectJson, "y"),
		jfloat(rectJson, "w"),
		jfloat(rectJson, "h"),
	};
}
