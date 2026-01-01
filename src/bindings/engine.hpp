#pragma once
#include <string>

namespace Etf {

struct Engine {
   public:
	static void LoadScene(const std::string& name = "");

   private:
	static void LoadAndPlayBGM(const std::string& name, float volume);
};
}  // namespace Etf
