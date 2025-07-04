#include <SDL3/SDL_main.h>
#include <Supergoon/engine.h>
#include <Supergoon/log.h>
#include <Supergoon/lua.h>

static void Start(void) {
	sgLogWarn("Running the lua file!");
	LuaRunFile("assets/lua/test.lua");
}

int main(int argc, char* argv[]) {
	SetStartFunction(Start);
	Run();
	return true;
}
