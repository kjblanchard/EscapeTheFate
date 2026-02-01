
#include <Supergoon/filesystem.h>
#include <Supergoon/sprite.h>

#include <bindings/SpriteAnimator.hpp>
#include <format>
#include <memory>
#include <unordered_map>
using namespace Etf;
using namespace std;

unordered_map<string, weak_ptr<AnimationData>> _loadedAnimationData;

static shared_ptr<AnimationData> createAnimationDataFromFile(const string& filename) {
	auto raw = CreateAnimationData();

	auto fullFilename = format("{}assets/aseprite/{}.json",
							   GetBasePath(), filename);

	CreateAnimationDataFromAsepriteFile(raw, fullFilename.c_str());

	return shared_ptr<AnimationData>(raw);
}

SpriteAnimator::SpriteAnimator(const string& filename, Sprite* sprite) {
	auto it = _loadedAnimationData.find(filename);
	if (it == _loadedAnimationData.end() || it->second.expired()) {
		auto shared = createAnimationDataFromFile(filename);
		_loadedAnimationData[filename] = shared;
		_animationData = shared;
	} else {
		_animationData = it->second.lock();
	}
	_animator = CreateAnimator(filename.c_str(), _animationData.get());
	_animator->Sprite = sprite;
}

SpriteAnimator::~SpriteAnimator() {
	DestroyAnimator(_animator);
}

void SpriteAnimator::PlayAnimationThenLoopSecond(const std::string& first, const std::string& second) {
	PlayAnimation(_animator, first.c_str(), 1);
	AddAnimationToAnimatorQueue(_animator, second.c_str(), -1);
}

void SpriteAnimator::StartAnimation(const std::string& animName, int loops, float speed) {
	_animator->AnimationSpeed = speed;
	PlayAnimation(_animator, animName.c_str(), loops);
}

void SpriteAnimator::UpdateAnimatorSpeed(float speed) {
	_animator->AnimationSpeed = speed;
}

void SpriteAnimator::AddAnimationToQueue() {
}
