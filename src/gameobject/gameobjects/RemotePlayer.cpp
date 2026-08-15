#include <Supergoon/json.h>
#include <Supergoon/sprite.h>
#include <sgtools/log.h>

#include <cmath>
#include <engine.hpp>
#include <gameState.hpp>
#include <gameobject/gameobjects/RemotePlayer.hpp>
#include <systems/GameObjectSystem.hpp>

using namespace Etf;

RemotePlayer* RemotePlayer::Instance_ = nullptr;

static const float sLerpSpeed = 10.0f;

struct RemoteCharData {
	std::string sprite;
	int frameW;
	int frameH;
};

static RemoteCharData getCharDataByIndex(uint8_t index) {
	auto obj = Engine::Json::GetJsonObjectFromDirectory("playerCharacters");
	if (!obj) return {"player1", 26, 36};
	auto count = jGetObjectArrayLength(obj);
	if (index >= count) index = 0;
	auto entry = jGetObjectInObjectWithIndex(obj, index);
	RemoteCharData data;
	data.sprite = jstr(entry, "overworldSprite");
	data.frameW = jint(entry, "overworldFrameW");
	data.frameH = jint(entry, "overworldFrameH");
	if (data.frameW == 0) data.frameW = 32;
	if (data.frameH == 0) data.frameH = 32;
	jReleaseObjectFromFile(obj);
	return data;
}

void RemotePlayer::SpawnForOnline(uint8_t characterIndex) {
	if (Instance_) return;
	auto charData = getCharDataByIndex(characterIndex);
	auto* rp = new RemotePlayer(charData.sprite, charData.frameW, charData.frameH);
	Instance_ = rp;
	AddGameObjectToGameObjectSystem(rp);
	sgLogInfo("RemotePlayer spawned with character index %d", characterIndex);
}

void RemotePlayer::ApplyNetworkMove(float x, float y, uint8_t direction, bool moving) {
	if (!Instance_) return;
	Instance_->TargetX_ = x;
	Instance_->TargetY_ = y;
	Instance_->Direction_ = direction;
	Instance_->Moving_ = moving;
}

void RemotePlayer::DestroyRemote() {
	if (!Instance_) return;
	Instance_->ShouldBeDestroyed_ = true;
	Instance_ = nullptr;
}

RemotePlayer::RemotePlayer(const std::string& spriteName, int frameW, int frameH)
	: GameObject(0, 0) {
	auto filename = spriteName + ".png";
	float fw = (float)frameW;
	float fh = (float)frameH;
	Sprite_ = Engine::Sprites::CreateSpriteFull(filename, &X_, &Y_, {0, 0, fw, fh}, {0, 0, fw, fh});
	Animator_ = std::make_unique<SpriteAnimator>(spriteName, Sprite_);
	Animator_->StartAnimation("walkD");
}

RemotePlayer::~RemotePlayer() {
	if (Instance_ == this) Instance_ = nullptr;
	DestroySprite(Sprite_);
}

void RemotePlayer::Start() {}

void RemotePlayer::Update() {
	float dx = TargetX_ - X_;
	float dy = TargetY_ - Y_;
	float dist = std::sqrt(dx * dx + dy * dy);

	if (dist > 0.5f) {
		float t = sLerpSpeed * GameState::DeltaTimeSeconds;
		if (t > 1.0f) t = 1.0f;
		SetX(X_ + dx * t);
		SetY(Y_ + dy * t);
	} else {
		SetX(TargetX_);
		SetY(TargetY_);
	}

	if (Moving_) {
		Animator_->UpdateAnimatorSpeed(1.0f);
	} else {
		Animator_->UpdateAnimatorSpeed(0.0f);
	}

	if (Direction_ != LastAnimDir_) {
		LastAnimDir_ = Direction_;
		Animator_->StartAnimation(animNameFromDir(Direction_));
	}
}

void RemotePlayer::Draw() {}

const char* RemotePlayer::animNameFromDir(uint8_t dir) {
	switch (dir) {
		case 2: return "walkU";
		case 1: return "walkR";
		case 0: return "walkD";
		case 3: return "walkL";
	}
	return "walkD";
}
