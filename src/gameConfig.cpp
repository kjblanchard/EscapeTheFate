#include <Supergoon/log.h>

#include <bindings/engine.hpp>
#include <gameConfig.hpp>
#include <string>

using namespace std;
using namespace Etf;

gameConfig GameConfig::_config;

void GameConfig::LoadGameConfig(const std::string& configFileName) {
	auto ec = glz::read_file_json(_config, "./assets/config/gameConfig.jsonc", std::string{});
	if (ec) {
		sgLogError("Error reading config file, please make sure it is in assets folder/gameConfig.jsonc!");
		return;
	}
}
