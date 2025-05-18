#include <SDL3/SDL_main.h>
#include <Supergoon/Audio/Audio.h>
#include <Supergoon/Input/keyboard.h>
#include <Supergoon/Input/mouse.h>
#include <Supergoon/UI/ui.h>
#include <Supergoon/UI/uiimage.h>
#include <Supergoon/UI/uilayoutgroup.h>
#include <Supergoon/UI/uiobject.h>
#include <Supergoon/UI/uirect.h>
#include <Supergoon/UI/uitext.h>
#include <Supergoon/engine.h>
#include <Supergoon/gameobject.h>
#include <Supergoon/graphics.h>
#include <Supergoon/log.h>
#include <Supergoon/lua.h>
#include <Supergoon/map.h>
#include <Supergoon/window.h>
#include <stdio.h>

// static void CreatePlayer(void* userdata, GameObject* go) {
// 	TiledObject* mapData = (TiledObject*)userdata;
// 	if (!mapData) {
// 		return;
// 	}
// 	go->X = mapData->X;
// 	go->Y = mapData->Y;
// 	go->W = mapData->Width;
// 	go->H = mapData->Height;
// }
// static void StartPlayer(GameObject* go) {
// 	(void)go;
// }
// static void UpdatePlayer(GameObject* go) {
// 	(void)go;
// 	if (IsKeyboardKeyJustPressed(SDL_SCANCODE_W)) {
// 		sgLogWarn("W key is just pressed!");
// 	}
// }

static void Start(void) {
	LuaRunFile("assets/lua/test.lua");
}

// static void Update(void) {
// }

int main(int argc, char* argv[]) {
	SetStartFunction(Start);
	// SetUpdateFunction(Update);
	Run();
}
