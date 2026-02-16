#include <bindings/Player.hpp>
using namespace Etf;

const Controller& Player::GetController() const {
	return Controller_;
}
