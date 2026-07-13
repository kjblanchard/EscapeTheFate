#pragma once
#include <Supergoon/Primitives/Color.h>
#include <Supergoon/Primitives/rectangle.h>

#include <functional>
#include <string>
#include <systems/SystemCallbacks.hpp>
struct Sprite;
struct Text;
struct sgGameObject;
typedef struct Texture Texture;

// This function should be created in the main.cpp, so that it can be used to initialize the game properly.
extern void InitializeGame();

namespace Etf {
// Loading states used internally for loading without delays
enum class CurrentSceneLoadingState {
	NotLoading,
	NextSceneQueued,
	WaitingForFadeOut,
	LoadingStart,
	LoadingMap,
	LoadingGameObjects,
	LoadingUI,
	LoadingDialog,
	LoadingFinish,
	JustLoaded,
	FadingIn,
	FadingInAllowUpdate,
	Count,
};

enum class LoadingScreenFadeTypes {
	NotFading,
	FadeIn,
	FadeOut,
};

namespace Engine {
// Called by main.cpp to set the current systems.
void RegisterSystems(const std::vector<SystemCallbacks>& systems);
const std::string& CurrentSceneName();
// Empty string will load the default scene from gameconfig.
void LoadScene(const std::string& name = "", float fadeOutTime = 1.0f, float fadeInTime = 1.0f, bool playTransitionSound = true);
static inline bool CheckForRectCollision(RectangleF& lhs, RectangleF& rhs) {
	return lhs.x < rhs.x + rhs.w && lhs.x + lhs.w > rhs.x && lhs.y < rhs.y + rhs.h && lhs.y + lhs.h > rhs.y;
}

namespace Debug {
void DrawRectPrimitive(RectangleF& rect, Color color = {255, 0, 0, 255}, bool filled = false, bool cameraOffset = true);
}

namespace Textures {
void LoadTextureFromBuffer(Texture* tex, const std::string& name);
}

namespace Sprites {
// TODO Do we even need this anymore?  Probably not
Sprite* CreateSpriteFull(const std::string& name, float* followX, float* followY, RectangleF sourceRect, RectangleF offsetSizeRect);
// Used for UI mainly, cause we need to handle drawing it outselves on top of everything
Sprite* CreateManualSpriteFull(const std::string& name, float* followX, float* followY, RectangleF sourceRect, RectangleF offsetSizeRect);
void SetSpriteVisible(Sprite* sprite, bool visible);

}  // namespace Sprites

namespace Audio {
void PlayBGM(const std::string& name, float volume = 1.0f);
void PlayBGMBackground(const std::string& name, float volume = 1.0f);
void PlaySFXBuffer(const std::string& name, float volume);
void SetGlobalBGMVolume(float volume);
void StopBGMBackground();
}  // namespace Audio
   //
namespace Tweening {
enum class TweenEaseTypes {
	Linear,
	QuintOut
};
float GetTweenedValue(float start, float end, float timeSeconds, float totalSeconds, TweenEaseTypes ease = TweenEaseTypes::Linear);
}  // namespace Tweening

namespace TextUtils {
Text* CreateText(const std::string& fontName, unsigned int fontSize, RectangleF location, const std::string& text, unsigned int numChars, bool centeredX, bool centeredY);
void DrawText(Text* text, float xOffset, float yOffset, Color& color);
}  // namespace TextUtils

namespace Json {
void GetJsonBufferFromDirectory(const char* name, char** buf, size_t* sz);
RectangleF GetRectFromObject(void* object, const std::string& key);
template <typename Lambda>
void jforeach_lambda(void* obj, Lambda&& lambda) {
	struct Wrapper {
		Lambda* fn;
	};
	Wrapper wrapper{&lambda};

	jforeach_obj(
		obj,
		[](const char* key, void* value, void* userData) {
			auto* w = static_cast<Wrapper*>(userData);
			(*w->fn)(key, value);
		},
		&wrapper);
}
}  // namespace Json

namespace DebugUI {
void Start();
void AddTab(std::function<void()> func);
void AddTab(const std::vector<std::function<void()>>& funcs);
void HandleEvent(void* event);
void Draw();
void Render();

}  // namespace DebugUI

}  // namespace Engine
}  // namespace Etf
