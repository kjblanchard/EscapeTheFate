#include <Supergoon/Graphics/graphics.h>
#include <Supergoon/Graphics/shader.h>
#include <Supergoon/Graphics/texture.h>
#include <sgtools/log.h>

#include <engine.hpp>
#include <gameConfig.hpp>
#include <gameState.hpp>
#include <systems/SplashSystem.hpp>
#include <ui/ui.hpp>
#include <vector>

using namespace Etf;

namespace {

constexpr float FADE_IN_TIME = 0.75f;
constexpr float HOLD_TIME = 1.0f;
constexpr float FADE_OUT_TIME = 0.75f;
constexpr int LOGICAL_W = 480;
constexpr int LOGICAL_H = 270;

enum class SplashState { Inactive,
						 FadingIn,
						 Holding,
						 FadingOut,
						 Done };

struct LogoEntry {
	Texture* texture;
	int texW, texH;
	float drawX, drawY, drawW, drawH;
};

std::vector<LogoEntry> logos_;
int currentLogoIndex_ = 0;
SplashState state_ = SplashState::Inactive;
float elapsed_ = 0.0f;
uint8_t currentAlpha_ = 0;
bool active_ = false;

void advanceToNextLogo() {
	++currentLogoIndex_;
	if (currentLogoIndex_ >= (int)logos_.size()) {
		state_ = SplashState::Done;
		active_ = false;
		//Hack to try and hide the UI, need to fix this in a //TODO
		auto niner = UI::GetRootUIObject()->GetChildByName("TitleNineSlice");
		auto ninertwo = UI::GetRootUIObject()->GetChildByName("MenuNineSlice");
		if (!niner || !ninertwo) sgLogCritical("Can not do it!");
		auto color = Color{255, 255, 255, 0};
		GraphicsUpdateFBOColor(&color);
		niner->SetVisible(true);
		ninertwo->SetVisible(true);
		//end hack
		Engine::LoadScene("", 0.1f, 1.75f, false);
		return;
	}
	state_ = SplashState::FadingIn;
	elapsed_ = 0.0f;
	currentAlpha_ = 0;
}

}  // namespace

void SplashSystem::Start() {
	auto& config = GameConfig::GetGameConfig();
	if (config.logos.empty()) {
		state_ = SplashState::Done;
		return;
	}
	for (const auto& name : config.logos) {
		auto fullName = name + ".png";
		Texture* tex = TextureCreate(fullName.c_str());
		Engine::Textures::LoadTextureFromBuffer(tex, name);
		int texW = TextureGetWidth(tex);
		int texH = TextureGetHeight(tex);
		int d = 1;
		while (texW / (d + 1) > 0 && texH / (d + 1) > 0 &&
			   (texW / d > LOGICAL_W || texH / d > LOGICAL_H)) {
			++d;
		}
		float drawW = (float)(texW / d);
		float drawH = (float)(texH / d);
		float drawX = (LOGICAL_W - drawW) / 2.0f;
		float drawY = (LOGICAL_H - drawH) / 2.0f;
		logos_.push_back({tex, texW, texH, drawX, drawY, drawW, drawH});
	}
	currentLogoIndex_ = 0;
	state_ = SplashState::FadingIn;
	elapsed_ = 0.0f;
	currentAlpha_ = 0;
	active_ = true;
}

void SplashSystem::Update() {
	if (!active_) return;
	elapsed_ += GameState::DeltaTimeSeconds;
	switch (state_) {
		case SplashState::FadingIn: {
			float alpha = Engine::Tweening::GetTweenedValue(0.0f, 255.0f, elapsed_, FADE_IN_TIME);
			currentAlpha_ = (uint8_t)(alpha > 255.0f ? 255 : alpha);
			if (elapsed_ >= FADE_IN_TIME) {
				currentAlpha_ = 255;
				elapsed_ = 0.0f;
				state_ = SplashState::Holding;
			}
			break;
		}
		case SplashState::Holding:
			if (elapsed_ >= HOLD_TIME) {
				elapsed_ = 0.0f;
				state_ = SplashState::FadingOut;
			}
			break;
		case SplashState::FadingOut: {
			float alpha = Engine::Tweening::GetTweenedValue(255.0f, 0.0f, elapsed_, FADE_OUT_TIME);
			currentAlpha_ = (uint8_t)(alpha < 0.0f ? 0 : alpha);
			if (elapsed_ >= FADE_OUT_TIME) {
				currentAlpha_ = 0;
				advanceToNextLogo();
			}
			break;
		}
		default:
			break;
	}
}

void SplashSystem::Draw() {
	if (!active_ || currentLogoIndex_ >= (int)logos_.size()) return;
	auto& logo = logos_[currentLogoIndex_];
	RectangleF dst = {logo.drawX, logo.drawY, logo.drawW, logo.drawH};
	RectangleF src = {0, 0, (float)logo.texW, (float)logo.texH};
	Color color = {255, 255, 255, currentAlpha_};
	DrawTexture(logo.texture, GetDefaultShader(), &dst, &src, 0, 1.0f, 0, &color);
}
