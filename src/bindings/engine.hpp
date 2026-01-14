#pragma once
#include <Supergoon/Primitives/Color.h>
#include <Supergoon/Primitives/rectangle.h>

#include <string>
struct Sprite;
struct Text;
struct sgGameObject;

namespace Etf {

struct Engine {
   public:
	static void LoadScene(const std::string& name = "");
	static Sprite* CreateSpriteFull(const std::string& name, sgGameObject* parent, RectangleF sourceRect, RectangleF offsetSizeRect);
	static void SetSpriteVisible(Sprite* sprite, bool visible);
	static void DrawRectPrimitive(RectangleF& rect, Color color = {255, 0, 0, 255}, bool filled = false, bool cameraOffset = true);
	static void HandleMapLoad();

	struct Audio {

		static void PlayBGMBackground(const std::string& name, float volume = 1.0f);
		static void StopBGMBackground();

	} Audio;

	struct Animation {
		static unsigned int CreateAnimatorFull(const std::string& name, Sprite* sprite);
		static void StartAnimatorAnimation(unsigned int animator, const char*, float animSpeed = 1.0);
		static void UpdateAnimatorAnimationSpeed(unsigned int animator, float animSpeed);
		static void DestroyAnimatorFull(unsigned int animator);

	} Animation;

	struct Tweening {
		static float GetTweenedValue(float start, float end, float timeSeconds, float totalSeconds);
	} Tweening;

	struct TextBoi {
		static Text* CreateText(const std::string& fontName, unsigned int fontSize, RectangleF location, const std::string& text, unsigned int numChars, bool centeredX, bool centeredY);
		static void DrawText(Text* text, float xOffset, float yOffset);
	} Textboi;

	struct Json {
		template <typename Lambda>
		static void jforeach_lambda(void* obj, Lambda&& lambda) {
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
	};

	// function engine.Collision.CheckForCollision(a, b)
	// return a.x < b.x + b.w and
	// a.x + a.w > b.x and
	// a.y < b.y + b.h and
	// a.y + a.h > b.y
	// end

	static inline bool CheckForRectCollision(RectangleF& lhs, RectangleF& rhs) {
		return lhs.x < rhs.x + rhs.w && lhs.x + lhs.w > rhs.x && lhs.y < rhs.y + rhs.h && lhs.y + lhs.h > rhs.y;
	}

   private:
	static std::string _nextScene;
	static void LoadAndPlayBGM(const std::string& name, float volume);
	static void loadSceneInternal();
};

}  // namespace Etf
