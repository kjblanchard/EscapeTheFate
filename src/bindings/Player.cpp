#include <bindings/Player.hpp>
using namespace Etf;

static int sCurrentPlayerNum_ = 0;

Player::Player(): PlayerNum_(sCurrentPlayerNum_++) {

}

const Controller& Player::GetController() const {
	return *Controller_;
}
