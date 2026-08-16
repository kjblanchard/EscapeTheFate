#pragma once
#include <components/SpriteAnimator.hpp>
#include <gameobject/GameObject.hpp>
#include <memory>
#include <string>
struct Sprite;

namespace Etf {

class RemotePlayer : public GameObject {
   public:
	static void SpawnForOnline(uint8_t characterIndex);
	static void ApplyNetworkMove(float x, float y, uint8_t direction, bool moving);
	static void DestroyRemote();
	static bool HasInstance();
	~RemotePlayer() override;

   private:
	RemotePlayer(const std::string& spriteName, int frameW, int frameH);
	void Start() override;
	void Update() override;
	void Draw() override;
	const char* animNameFromDir(uint8_t dir);

	static RemotePlayer* Instance_;
	Sprite* Sprite_ = nullptr;
	std::unique_ptr<SpriteAnimator> Animator_;
	float TargetX_ = 0;
	float TargetY_ = 0;
	bool Moving_ = false;
	uint8_t Direction_ = 0;
	uint8_t LastAnimDir_ = 255;
};

}  // namespace Etf
